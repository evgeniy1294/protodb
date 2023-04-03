-- NMEA parser (for GNSS only)s.

function getSatelliteSystem(msg)
    local code = string.char(msg[2]) .. string.char(msg[3])

    if code == "GP" then
        return "[GP] GPS"
    elseif code == "GL" then
        return "[GL] GLONASS"
    elseif code == "GA" then
        return "[GA] Galileo"
    elseif code == "DB" then
        return "[DB] BeiDou"
    elseif code == "GB" then
        return "[GB] BeiDou"
    elseif code == "GN" then
        return "[GN] GLONASS + GPS"
    else 
        return "["..code.."]".."Unknown"
    end
end

function getMessageFormat(msg)
    local code = string.char(msg[4]) .. string.char(msg[5]) .. string.char(msg[6])
    
    if code == "TXT" then
        return "[TXT] Text Transmission"
    elseif code == "RMC" then
        return "[RMC] Recommended Minimum Specific GNSS Data"
    elseif code == "VTG" then
        return "[VTG] Course Over Ground and Ground Speed"
    elseif code == "GGA" then
        return "[GGA] Global Positioning System Fix Data"
    elseif code == "GSA" then
        return "[GSA] GNSS DOP and Active Satellites"
    elseif code == "GSV" then
        return "[GSV] GNSS Satellites in View"
    elseif code == "GLL" then 
        return "[GLL] Geographic Position - Latitude/Longitude"
    else 
        return "["..code.."]".."Unknown"
    end
end

function parseAsRMC(msg)
    
end

function split(s,re)
    local i1 = 1
    local ls = {}
    local append = table.insert
    if not re then re = '%s+' end
    if re == '' then return {s} end
    while true do
        local i2,i3 = s:find(re,i1)
        if not i2 then
            local last = s:sub(i1)
            if last ~= '' then append(ls,last) end
            if #ls == 1 and ls[1] == '' then
                return {}
            else
                return ls
            end
        end
        append(ls,s:sub(i1,i2-1))
        i1 = i3+1
    end
end

function start()
    log:print("Seance started")
    
    s = "hello,world,from,,Lua"
    local data = split(s, ',')
    
    for w in data do
        if #w == 0 then log:print("zero")
        else log:print(w) end
    end
end

function stop()
    log:print("Seance stoped")
end

function beforeTransmit(msg)

end

function afterReceive(msg)
    local text = "Satellite system: " .. getSatelliteSystem(msg) .. '\n'
    text = text .. "Format: " .. getMessageFormat(msg) .. '\n'
    
    log:print(text)
end
 
