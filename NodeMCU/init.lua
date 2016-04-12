my_network = {
  essid = "";    -- your ESSID name
  password = ""; -- your ESSID password
}

print("Setting up WiFi...")
wifi.setmode(wifi.STATION)
wifi.sta.config(my_network.essid, my_network.password)
wifi.sta.connect()

abort = false;

tmr.alarm(0, 5000, 1, function()
  sta_status = wifi.sta.status()
  
  if sta_status == 1 then     --1: STATION_CONNECTING
    print("Connecting...")
  elseif sta_status == 5 then --5: STATION_GOT_IP
    tmr.stop(0)
    print("Done.")
    if abort == true then
      print("Aborted.")
      return
    end
    
    print("Loading script...")
    dofile("serial.lua")
  end 
end)
