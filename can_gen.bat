@echo off
setlocal enabledelayedexpansion

:: Устанавливаем кодировку UTF-8
chcp 65001 >nul
:: Устанавливаем кодировку Windows-1251
::chcp 1251 >nul

:: Устанавливаем имя узла (можно изменить на нужное)
set NODE_NAME=PANEL

:: Преобразуем в нижний регистр для имен файлов
set NODE_NAME_LOWER=!NODE_NAME!

call :toLower NODE_NAME_LOWER %NODE_NAME%

set H_FILE=can_node_%NODE_NAME_LOWER%_bus0.h
set C_FILE=can_node_%NODE_NAME_LOWER%_bus0.c

echo Используется узел: %NODE_NAME%
echo.

:: Запускаем генератор с переменной
C:\stm\vent-common\DBC\dbccangen.exe C:\stm\vent-common\DBC\vent.dbc %NODE_NAME% 0

:: Проверяем результат выполнения
if errorlevel 1 (
    echo Ошибка при генерации!
    pause
    exit /b 1
)

echo.
echo Готово!
pause

:: Сама функция преобразования
goto :eof

:toLower
set %1=%2
set %1=!%1:A=a!
set %1=!%1:B=b!
set %1=!%1:C=c!
set %1=!%1:D=d!
set %1=!%1:E=e!
set %1=!%1:F=f!
set %1=!%1:G=g!
set %1=!%1:H=h!
set %1=!%1:I=i!
set %1=!%1:J=j!
set %1=!%1:K=k!
set %1=!%1:L=l!
set %1=!%1:M=m!
set %1=!%1:N=n!
set %1=!%1:O=o!
set %1=!%1:P=p!
set %1=!%1:Q=q!
set %1=!%1:R=r!
set %1=!%1:S=s!
set %1=!%1:T=t!
set %1=!%1:U=u!
set %1=!%1:V=v!
set %1=!%1:W=w!
set %1=!%1:X=x!
set %1=!%1:Y=y!
set %1=!%1:Z=z!
goto :eof