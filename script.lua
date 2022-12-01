function start()
    log:print("Seance started")
end

function stop()
    log:print("Seance stoped")
end

function beforeTransmit(msg)
	text = "Bytes for transmit:\n"
	for k=1,#msg do
		text = text.."byte["..(k-1).."] = "..string.format("%x", msg[k])..'\n'
		msg[k] = msg[k] + 1
	end
	
	text = text.."Increment these bytes before transmit"
	
    log:print(text)
end

function afterReceive(msg)
	text = "Cuptured data:\n"
	for k=1,#msg do
		text = text.."byte["..(k-1).."] = "..string.format("%x", msg[k])..'\n'
		msg[k] = msg[k] + 1
	end
    
    log:print(text)
end

function buildMessage(msg)
    --log:print("Example: buildMessage", msg[1], msg[2], msg[3])
end
