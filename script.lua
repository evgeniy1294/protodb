
function start()
    log:print("Example: Start connection")
end

function stop()
    --log:clear()
end

function beforeTransmit(msg)
    msg[1] = 0x17
    text = "Byte[0] = "..string.format("%x", msg[1])..'\n'
    text = text.."Byte[1] = "..string.format("%x", msg[2])..'\n'
    text = text.."Byte[2] = "..string.format("%x", msg[3])..'\n'
    text = text.."Byte[3] = "..string.format("%x", msg[4])
    log:print(text)
end

function afterReceive(msg)
    --msg[1] = 0x31
    log:print("Example: After receive")
    error("TEST ERROR")
end

function buildMessage(msg)
    --log:print("Example: buildMessage", msg[1], msg[2], msg[3])
end
