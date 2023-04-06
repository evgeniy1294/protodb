-- NMEA parser (for GNSS only).

function split(s,re)
    local i1 = 1
    local ls = {}
    local append = table.insert
    
    if not re then re = '%s+' end
    
    if re == '' then 
        return {s}
    end
    
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

function getFormatDetail(code)
    if code == "TXT" then
        return code, "[TXT] Text Transmission"
    elseif code == "RMC" then
        return code, "[RMC] Recommended Minimum Specific GNSS Data"
    elseif code == "VTG" then
        return code, "[VTG] Course Over Ground and Ground Speed"
    elseif code == "GGA" then
        return code, "[GGA] Global Positioning System Fix Data"
    elseif code == "GSA" then
        return code, "[GSA] GNSS DOP and Active Satellites"
    elseif code == "GSV" then
        return code, "[GSV] GNSS Satellites in View"
    elseif code == "GLL" then 
        return code, "[GLL] Geographic Position - Latitude/Longitude"
    else 
        return code, "["..code.."]".."Unknown"
    end
end

function parseAsRMC(msg)
    local fields = split(msg, ",")
    
    local text = ""
    
    -- Time field
    if fields[2] ~= "" then 
        text = text .. "Time: " .. fields[2] .. "\n"
    else
        text = text .. "Time: unknown" .. "\n"
    end
    
    -- Date field
    if fields[10] ~= "" then 
        local d = fields[10][1] .. fields[10][2]
        local m = fields[10][3] .. fields[10][4]
        local y = fields[10][5] .. fields[10][6]
        
        text = text .. "Date: " .. d .. "." .. m .. "." .. y .. "\n"
    else
        text = text .. "Date: unknown" .. "\n"
    end
    
    
    -- Status field
    if fields[3] == "A" then
        text = text .. "Status: Data valid\n"
    else
        text = text .. "Status: Navigation receiver warning\n"
    end
    
    -- Latitude [N/S] field
    if fields[4] ~= "" and fields[5] ~= "" then 
        text = text .. "Latitude [N/S]: " .. fields[4] .. ", " .. fields[5] .. "\n"
    else
        text = text .. "Latitude [N/S]: unknown" .. "\n"
    end
    
    -- Longitude [E/W] field
    if fields[6] ~= "" and fields[7] ~= "" then 
        text = text .. "Longitude [E/W]: " ..
                fields[6] .. ", " .. fields[7] .. "\n"
    else
        text = text .. "Longitude [E/W]: unknown" .. "\n"
    end
    
    -- Speed over ground, knots
    if fields[8] ~= "" then 
        text = text .. "Speed [knots]: " .. fields[8] .. "\n"
    else
        text = text .. "Speed [knots]: unknown" .. "\n"
    end
    
    -- Course Over Ground, degrees
    if fields[9] ~= "" then 
        text = text .. "Course Over Ground [deg]: " .. fields[9] .. "\n"
    else
        text = text .. "Course Over Ground [deg]: unknown" .. "\n"
    end
    
    -- Magnetic variation, degrees E/W
    if fields[11] ~= ""  and fields[12] ~= "" then 
        text = text .. "Magnetic variation [deg E/W]: " .. 
                            fields[11] .. "," .. fields[12]"\n"
    else
        text = text .. "Magnetic variation [deg E/W]: unknown" .. "\n"
    end
    
    -- Mode
    if fields[13] == "A" then 
        text = text .. "Mode: Autonomous"
    elseif fields[13] == "D" then 
        text = text .. "Mode: Differential"
    elseif fields[13] == "E" then 
        text = text .. "Mode: Estimated (dead reckoning)"
    elseif fields[13] == "M" then 
        text = text .. "Mode: Manual input"
    elseif fields[13] == "S" then 
        text = text .. "Mode: Simulator"
    else
        text = text .. "Mode: Data not valid" .. "\n"
    end
    
    return text
end



function start()
    log:print("Seance started")
end

function stop()
    log:print("Seance stoped")
end

function beforeTransmit(msg)

end

function afterReceive(msg)
    local text = "Satellite system: " .. getSatelliteSystem(msg) .. '\n'
    
    local format = string.char(msg[4]) .. string.char(msg[5]) .. string.char(msg[6])
    text = text .. "Format: " .. getFormatDetail(format) .. '\n'
    
    if format == "RMC" then text = text .. parseAsRMC(utils:toString(msg)) .. '\n' end
    log:print(text)
end
 
