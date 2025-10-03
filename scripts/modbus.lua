local READ_COILS <const> = 0x01
local READ_DISCRETE_INPUTS <const> = 0x02
local READ_HOLDING_REGISTERS <const> = 0x03
local READ_INPUT_REGISTERS <const> = 0x04
local WRITE_SINGLE_COIL <const> = 0x05
local WRITE_SINGLE_REGISTER <const> = 0x06
local READ_EXCEPTION_STATUS <const> = 0x07
local DIAGNOSTICS <const> = 0x08
local WRITE_MULTIPLE_COILS <const> = 0x0F
local WRITE_MULTIPLE_REGISTERS <const> = 0x10
local READ_WRITE_MULTIPLE_REGISTERS <const> = 0x17

function commandToString(code)
    if code == READ_COILS then
        return "Read Coils"
    elseif code == READ_DISCRETE_INPUTS then
        return "Read Discrete Inputs"
    elseif code == READ_HOLDING_REGISTERS then
        return "Read Holding Registers"
    elseif code == READ_INPUT_REGISTERS then
        return"Read Input Registers"
    elseif code == WRITE_SINGLE_COIL then
        return "Write Single Coil"
    elseif code == WRITE_SINGLE_REGISTER then
        return "Write Single Register"
    elseif code == READ_EXCEPTION_STATUS then
        return "Read Exception Status"
    elseif code == DIAGNOSTICS then
        return "Diagnostics"
    elseif code == 0x0B then
        return "Get Comm Event Counter"
    elseif code == 0x0C then
        return "Get Comm Event Log"
    elseif code == WRITE_MULTIPLE_COILS then
        return "Write Multiple Coils"
    elseif code == WRITE_MULTIPLE_REGISTERS then
        return "Write Multiple registers"
    elseif code == 0x11 then
        return "Report Slave ID"
    elseif code == 0x14 then
        return "Read File Record"
    elseif code == 0x15 then
        return "Write File Record"
    elseif code == 0x16 then
        return "Mask Write Register"
    elseif code == READ_WRITE_MULTIPLE_REGISTERS then
        return "Read/Write Multiple registers"
    elseif code == 0x18 then
        return "Read FIFO Queue"
    elseif code == 0x2B then
        return "Encapsulated Interface Transport"
    else
        return "Unknown"
    end
end

function errorToString(code)
    if code == 0x01 then
        return "Illegal function"
    elseif code == 0x02 then
        return "Illegal data address"
    elseif code == 0x03 then
        return "Illegal data value"
    elseif code == 0x04 then
        return "Slave device failure"
    elseif code == 0x05 then
        return "Acknowledge"
    elseif code == 0x06 then
        return "Slave device busy"
    elseif code == 0x08 then
        return "Memory parity error"
    elseif code == 0x0A then
        return "Gateway path unavailable"
    elseif code == 0x0B then
        return "Gateway target device failed to respond"
    else
        return "Unknown"
    end
end

function diagnosticSubFunctionToString(code)
    if code == 0x00 then
        return "Return Query Data"
    elseif code == 0x01 then
        return "Restart Communications Option"
    elseif code == 0x02 then
        return "Return Diagnostic Register"
    elseif code == 0x03 then
        return "Change ASCII Input Delimiter"
    elseif code == 0x04 then
        return "Force Listen Only Mode"
    elseif code == 0x0A then
        return "Clear Counters and Diagnostic Register"
    elseif code == 0x0B then
        return "Return Bus Message Count"
    elseif code == 0x0C then
        return "Return Bus Communication Error Count"
    elseif code == 0x0D then
        return "Return Bus Exception Error Count"
    elseif code == 0x0E then
        return "Return Slave Message Count"
    elseif code == 0x0F then
        return "Return Slave No Response Count"
    elseif code == 0x10 then
        return "Return Slave NAK Count"
    elseif code == 0x11 then
        return "Return Slave Busy Count"
    elseif code == 0x12 then
        return "Return Bus Character Overrun Count"
    elseif code == 0x14 then
        return "Clear Overrun Counter and Flag"
    else
        return "Unknown"
    end
end

function parseDiagnosticResult(msg, sub_func)
    local str = ""
    if sub_func == 0x00 then
       str = str.."Data - ".."0x"..string.format("%x", msg[5])..string.format("%x", msg[6])
    end

    return str
end

function parseResponce(msg)
    code = msg[2]
    local str = ""

    if code == READ_COILS or code == READ_DISCRETE_INPUTS then
        str = str.."Byte count - "..string.format("%x", msg[3])..'\n'
        str = str.."Values - { "
        for i = 4, #msg-2 do
            str = str..string.format("%x", msg[i])..' '
        end
        str = str.."}"

    elseif code == READ_HOLDING_REGISTERS or code == READ_INPUT_REGISTERS then
        str = str.."Byte count - "..string.format("%x", msg[3])..'\n'
        str = str.."Values - { \n"
        for i = 4, #msg-2, 2 do
            value = (msg[i] << 8) | msg[i+1]
            str = str.."  0x"..string.format("%x", value).." ("..string.format("%d", value)..") \n"
        end
        str = str.."}"

    elseif code == WRITE_SINGLE_COIL then
        str = str.."Coil address - ".."0x"..string.format("%x", msg[3])..string.format("%x", msg[4])..'\n'
        value = msg[5] << 8 | msg[6]

        if value == 0xFF00 then
            str = str.."Current value - ON"
        elseif value == 0x0000 then
            str = str.."Current value - OFF"
        else
            str = str.."Wrong value field"
        end

    elseif code == WRITE_SINGLE_REGISTER then
        str = str.."Register address - ".."0x"..string.format("%x", msg[3])..string.format("%x", msg[4])..'\n'
        str = str.."Current value - ".."0x"..string.format("%x", msg[5])..string.format("%x", msg[6])..'\n'

    elseif code == DIAGNOSTICS then
        sub_func = msg[3] << 8 | msg[4]
        str = str.."Sub function - "..diagnosticSubFunctionToString(sub_func)..'\n'
        str = str..parseDiagnosticResult(msg, sub_func)

    elseif code == WRITE_MULTIPLE_COILS then
        start_addr =  msg[3] << 8 | msg[4]
        count = msg[5] << 8 | msg[6]
        str = str.."Starting Address - ".."0x"..string.format("%x", start_addr)..'\n'
        str = str.."Quantity of Outputs - "..string.format("%d", count)

    elseif code == WRITE_MULTIPLE_REGISTERS then
        start_addr =  msg[3] << 8 | msg[4]
        count = msg[5] << 8 | msg[6]
        str = str.."Starting Address - ".."0x"..string.format("%x", start_addr)..'\n'
        str = str.."Quantity of Registers - "..string.format("%d", count)
    end

    return str
end

function writeSingleCoil(device, coil, value)
    local msg = {}
        msg[1] = device
        msg[2] = WRITE_SINGLE_COIL
        msg[3] = coil >> 8
        msg[4] = coil & 0xff

        if value == 0 then
            msg[5] = 0x00
        else
            msg[5] = 0xff
        end

        msg[6] = 0x00

    return msg
end

function writeSingleRegister(device, reg, value)
    local msg = {}
        msg[1] = device
        msg[2] = WRITE_SINGLE_REGISTER
        msg[3] = reg >> 8
        msg[4] = reg & 0xff
        msg[5] = value >> 8
        msg[6] = value & 0xff

    return msg
end

function writeMultipleRegisters(device, reg, values)
    local msg = {}
        msg[1] = device
        msg[2] = WRITE_MULTIPLE_REGISTERS
        msg[3] = reg >> 8
        msg[4] = reg & 0xff
        msg[5] = #values >> 8
        msg[6] = #values & 0xff
        msg[7] = #values * 2

        for i = 1, #values do
            msg[#msg + 1] = values[i] >> 8
            msg[#msg + 1] = values[i] & 0xff
        end

    return msg
end

function readCoils(device, coil, count)
    local msg = {}
        msg[1] = device
        msg[2] = READ_HOLDING_REGISTERS
        msg[3] = coil >> 8
        msg[4] = coil & 0xff
        msg[5] = count >> 8
        msg[6] = count & 0xff

    return msg
end

function readHoldingRegisters(device, reg, count)
    local msg = {}
        msg[1] = device
        msg[2] = READ_HOLDING_REGISTERS
        msg[3] = reg >> 8
        msg[4] = reg & 0xff
        msg[5] = count >> 8
        msg[6] = count & 0xff

    return msg
end

function readInputRegisters(device, reg, count)
    local msg = {}
        msg[1] = device
        msg[2] = READ_INPUT_REGISTERS
        msg[3] = reg >> 8
        msg[4] = reg & 0xff
        msg[5] = count >> 8
        msg[6] = count & 0xff

    return msg
end

function testLine(device)
    local msg = {}
        msg[1] = device
        msg[2] = DIAGNOSTICS
        msg[3] = 0x00
        msg[4] = 0x00
        msg[5] = 0xA5
        msg[6] = 0x37

    return msg
end

function start()
    crc = Checksum()
    crc:setWidth(16)
    crc:setPoly(0x8005)
    crc:setSeed(0xFFFF)
    crc:setReflectIn(true)
    crc:setReflectOut(true)
    crc:setXorOut(0)
    crc:finalize()

    log:print("Seance started")
end

function stop()
    log:print("Seance stoped")
end


function beforeTransmit(msg)
    cmdstr = "bt = "
    for i = 1, #msg do
        cmdstr = cmdstr .. string.char(msg[i])
    end

    -- Execute user function
    local func = load(cmdstr, "usrcall", "t", _ENV)
    if func ~= nil then
        func()

        -- Update message buffer
        msg:clear()
        for i = 1, #bt do
            msg[i] = bt[i]
        end

        -- Append checksum
        if #msg > 0 then
            crc:calculate(msg)
            local crc16 = crc:finalize()

            msg[#msg + 1] = crc16 & 0xff
            msg[#msg + 1] = (crc16 >> 8 ) & 0xff
        end
    end
end

function afterReceive(msg)
    local str = ""

    crc:calculate(msg)
    local crc16 = crc:finalize()

    if crc16 == 0 then
        str = str.."Command - "..commandToString(msg[2] & 0x7F)..'\n'
        str = str.."Device address - ".."0x"..string.format("%x", msg[1])..'\n'

        if msg[2] > 0x80 then
            str = str.."Error - "..errorToString(msg[3])
        else
            str = str..parseResponce(msg)
        end
    else
       str = "Error - Wrong CRC"
    end

    log:print(str)
end

