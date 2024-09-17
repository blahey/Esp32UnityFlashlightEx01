// test

using System;
using System.Net;
using System.Net.Sockets;
using System.Text;
using UnityEngine;

public class DataIO : MonoBehaviour
{
    public int flashlightButtonState;
    public int potentiometerValue;

    UdpClient udpClient;
    IPEndPoint remoteEndPoint;

    void Start()
    {
        udpClient = new UdpClient(4211);
        remoteEndPoint = new IPEndPoint(IPAddress.Any, 0);
        udpClient.BeginReceive(new AsyncCallback(ReceiveCallback), null);
    }

    void ReceiveCallback(IAsyncResult ar)
    {
        byte[] receivedBytes = udpClient.EndReceive(ar, ref remoteEndPoint);
        string receivedText = Encoding.ASCII.GetString(receivedBytes);
        Debug.Log("Received: " + receivedText);

        // Parse the received data
        string[] data = receivedText.Split(',');
        if (data.Length == 2)
        {
            int.TryParse(data[0].Split(':')[1].Trim(), out flashlightButtonState);
            int.TryParse(data[1].Split(':')[1].Trim(), out potentiometerValue);
        }

        udpClient.BeginReceive(new AsyncCallback(ReceiveCallback), null);
    }

    void OnApplicationQuit()
    {
        udpClient.Close();
    }
}
