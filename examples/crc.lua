crc = Checksum() -- Объект класса "Калькулятор CRC".

function start()
    -- Настройка калькулятора
    crc:setWidth(16)           -- Установка длина полинома в битах (от 3-х до 64-х).
    crc:setPoly(0x8005)        -- Значение полинома.
    crc:setSeed(0xFFFF)        -- Начальное значение.
    crc:setReflectIn(true)     -- Начало и направление вычислений.
    crc:setReflectOut(true)    -- Инверсия порядока битов регистра при входе на элемент XOR.
    crc:setXorOut(0)           -- Число, с которым складывается по модулю 2 полученный результат.
    crc:finalize()

    log:print("Seance start")
end

function stop()
    log:print("Seance stoped")
end

function beforeTransmit(msg)
    -- Расчёт контрольных сумм
    crc:calculate(msg)

    -- Получение рассчитанного значения и подготовка калькулятора к следующему циклу.
    local crc16 = crc:finalize()

    -- Разбиение значения контрольной суммы на два байта. Скобки {} обязательны, такой синтаксис для создания таблицы
    -- LUA не умеет в битовые операции, поэтому используются фукнции utils
    local bt = { utils:shortToBytes(crc16) }

    -- В языке LUA символ # перед именем массива вернёт его длинну. Начальный индекс массивов в LUA - 1.
    msg[#msg+1] = bt[1]
    msg[#msg+1] = bt[2]
end

function afterReceive(msg)
    -- Эта функция будет запущена после приёма сообщения. В ней можно проверить целостность входящего пакета.
    crc:calculate(msg)

    if crc:finalize() == 0 then
        log:print("CRC OK!")
    end
end
