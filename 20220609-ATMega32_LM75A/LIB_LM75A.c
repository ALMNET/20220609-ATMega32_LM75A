

#include "LIB_LM75A.h"



uint8_t LM75A_Write(uint8_t data)
{
	if(!I2C_Start(LM75A_WR_ADR) && (I2C_Write(data) != 2))
	{
		I2C_Stop();
		return 0;
	}
	
	else
	return 1;
}


uint8_t LM75A_Read()
{
    //PFC8574_Write(dataBitmask);

    uint8_t data;
    I2C_Start(LM75A_RD_ADR);
    data = I2C_Read_Ack();
    I2C_Read_Nack();						/* Read flush data with nack */
    I2C_Stop();	
	
	return data;
}













uint8_t PFC8574_Read(uint8_t dataBitmask)
{
    PFC8574_Write(dataBitmask);

    uint8_t data;
    I2C_Start(PCF8574_RD_ADR);
    data = I2C_Read_Ack();
    I2C_Read_Nack();						/* Read flush data with nack */
    I2C_Stop();	
	
	return data;
}