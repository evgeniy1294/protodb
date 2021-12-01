
function start()
    log:print("Example: Start connection")
end

function stop()
    log:clear()
end

function beforeTransmit(msg)
    __tx[1] = 0x12
    text = "Byte[0] = "..string.format("%x", string.byte(msg[1]))..'\n'
    text = text.."Byte[1] = "..string.format("%x", string.byte(msg[2]))..'\n'
    text = text.."Byte[2] = "..string.format("%x", string.byte(msg[3]))..'\n'
    text = text.."Byte[3] = "..string.format("%x", string.byte(msg[4]))
    log:print(text)
end

function afterReceive(msg)
    --log:print("Example: After receive", msg[1], msg[2], msg[3])
end

function buildMessage(msg)
    --log:print("Example: buildMessage", msg[1], msg[2], msg[3])
end
