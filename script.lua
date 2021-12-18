
function start()
    log:print("Start session")
end

function stop()
    log:print("Stop session")
end

function beforeTransmit(msg)
    text = "Byte[0] = "..string.format("%x", msg[1])..'\n'
    text = text.."Byte[1] = "..string.format("%x", msg[2])..'\n'
    text = text.."Byte[2] = "..string.format("%x", msg[3])..'\n'
    text = text.."Byte[3] = "..string.format("%x", msg[4])
    log:print(text)
end

function afterReceive(msg)
    text = "Byte[0] = "..string.format("%x", msg[1])..'\n'
    text = text.."Byte[1] = "..string.format("%x", msg[2])..'\n'
    text = text.."Byte[2] = "..string.format("%x", msg[3])..'\n'
    text = text.."Byte[3] = "..string.format("%x", msg[4])
    log:print(text)
end

function buildMessage(msg)
    --log:print("Example: buildMessage", msg[1], msg[2], msg[3])
end
