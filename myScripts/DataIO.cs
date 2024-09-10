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
