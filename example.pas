PROGRAM Example;


CONST
    MAX = 100;
    MIN = 0;

VAR
    num1, num2, sum: INTEGER;
    average: LONG;

PROCEDURE Calculate;
VAR
    temp: INTEGER;
BEGIN
    temp := num1 + num2;
    sum := temp;
    average := sum / 2
END;

BEGIN
    {主程序开始}
    READ(num1, num2);
    
    IF num1 > MAX THEN
        num1 := MAX;
    
    IF num2 < MIN THEN
        num2 := MIN;
    
    WHILE num1 <> num2 DO
        num1 := num1 - 1;
    
    Calculate;
    
    WRITE(sum, average)
END.