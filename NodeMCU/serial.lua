my_thingspeak = {
  api_key = ""; -- your ThingSpeak channel write API key
}

maj_v, min_v, dev_v = node.info()
user_agent = "NodeMCU "..maj_v.."."..min_v.."."..dev_v.." (ESP8266 Lua)" 

uart.setup(0, 9600, 8, 0, 1)

uart.on("data", "\n", 
  function(data)
    conn = net.createConnection(net.TCP, 0)
    conn:connect(80, "184.106.153.149") 
    
    params = string.gsub(data, "[\r\n]", "")
  
    conn:on("connection", function(conn)
      print("Sending data to ThingSpeak...")
      conn:send("POST /update?"..params.." HTTP/1.1\r\n")
      conn:send("Host: api.thingspeak.com\r\n")
      conn:send("X-THINGSPEAKAPIKEY: "..my_thingspeak.api_key.."\r\n")
      conn:send("Accept: */*\r\n")
      conn:send("Connection: close\r\n")
      conn:send("User-Agent: "..user_agent.."\r\n")
      conn:send("\r\n")
    end)
    
    conn:on("sent", function(conn)
      print("Data sent.")
    end)
    
    conn:on("receive", function(conn, payload)
      -- TODO: Check if success, but for now just print and close connection
      print(payload)
      conn:close()
    end)    
end, 0)
