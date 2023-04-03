function start()
    log:print("Seance started")
end

function stop()
    log:print("Seance stoped")
end

function beforeTransmit(msg)
    -- В обработчике beforeTransmit можно поработать с данными до их отправки.
    -- Например, добавить к посылке контрольные суммы.
    -- Помните, начальный индекс в массиве LUA - 1, а не 0.
end

function afterReceive(msg)
    -- Обработчик afterReceive служит для обработки принятых данных.
end
 
