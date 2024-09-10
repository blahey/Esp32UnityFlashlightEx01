// comment
// new comment in practiceBrancingMerging branch

using System;
using System.Net;
using System.Net.Sockets;
using System.Text;
using UnityEngine;

public class DataIO : MonoBehaviour
{
    private UdpClient udpClient;
    private UdpClient sendUdpClient;
    public string esp32IP = "192.168.0.27"; // Replace with your ESP32 IP address
    public int receivePort = 4210; // Port number for receiving data
    public int sendPort = 4211; // Port number for sending data
    public GameObject targetObject; // The game object to track

    public int sensorValue32; // Sensor data from pin 32
    public int sensorValue33; // Sensor data from pin 33
    public int sensorValue34; // Sensor data from pin 34
    public int sensorValue35; // Sensor data from pin 35
    public int flashlightButtonState; // Switch data from pin 26  
    public int rssiValue; // WiFi signal strength (RSSI)

    [System.Serializable]
    public class SensorData
    {
        public int sensorValue32;
        public int sensorValue33;
        public int sensorValue34;
        public int sensorValue35;
        public int flashlightButtonState;
        public int rssi;
    }

    [System.Serializable]
    public class PositionData
    {
        public float x;
        public float y;
        public float z;
    }

    void Start()
    {
        udpClient = new UdpClient(receivePort);
        udpClient.BeginReceive(new AsyncCallback(ReceiveCallback), null);
        sendUdpClient = new UdpClient();
        InvokeRepeating("SendPositionData", 0f, 0.04f); // Send position data every 40ms
    }

void ReceiveCallback(IAsyncResult ar)
{
    IPEndPoint endPoint = new IPEndPoint(IPAddress.Any, receivePort);
    byte[] receiveBytes = udpClient.EndReceive(ar, ref endPoint);
    string dataFromESP32 = Encoding.ASCII.GetString(receiveBytes);
    Debug.Log("Data from ESP32: " + dataFromESP32);

    try
    {
        string[] values = dataFromESP32.Split(',');
        sensorValue32 = int.Parse(values[0]);
        sensorValue33 = int.Parse(values[1]);
        sensorValue34 = int.Parse(values[2]);
        sensorValue35 = int.Parse(values[3]);
        flashlightButtonState = int.Parse(values[4]);
        rssiValue = int.Parse(values[5]);
        Debug.Log("Parsed Sensor Value 32: " + sensorValue32);
        Debug.Log("Parsed Sensor Value 33: " + sensorValue33);
        Debug.Log("Parsed Sensor Value 34: " + sensorValue34);
        Debug.Log("Parsed Sensor Value 35: " + sensorValue35);
        Debug.Log("Parsed flashlightButtonState: " + flashlightButtonState);
        Debug.Log("Parsed RSSI Value: " + rssiValue);
    }
    catch (Exception e)
    {
        Debug.LogWarning("Failed to parse data: " + e.Message);
    }

    // Restart receiving data
    udpClient.BeginReceive(new AsyncCallback(ReceiveCallback), null);
}

void SendPositionData()
{
    if (targetObject != null)
    {
        Vector3 position = targetObject.transform.position;
        string positionData = $"{position.x},{position.y},{position.z}";
        byte[] data = Encoding.ASCII.GetBytes(positionData);
        sendUdpClient.Send(data, data.Length, esp32IP, sendPort);
        Debug.Log("Position Data sent: " + positionData);
    }
}


    void OnApplicationQuit()
    {
        if (udpClient != null)
        {
            udpClient.Close();
        }
        if (sendUdpClient != null)
        {
            sendUdpClient.Close();
        }
    }
}
