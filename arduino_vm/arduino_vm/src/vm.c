/*
 * vm.c
 *
 * Created: 2016-12-7 9:54:41
 *  Author: winxos
 */ 
#include "public.h"
#include "uart.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define MAX_MEM 100
int MEM[MAX_MEM] = {0}, ADDER = 0; //ONLY MAX_MEN MEMORY, ONE REGISTER ADDER
#define MAX_STACK 30
int STACK[MAX_STACK] = {0};
enum
{
	INPUT = 10, PRINT, LOAD = 20, STORE, SET,
	ADD = 30, SUB, MUL, DIV, MOD, INC, DEC, NEG,
	JMP = 40, JMPN, JMPZ, HALT,
	AND = 50, OR, XOR,
	PUSH = 60, POP
};

#define COLS 10

void dump();
void runSML();
uint16_t input_code();
uint16_t Scanf_data();



void dump() //Pretty Show Memory For Debugging
{
	uint16_t i;
	uart_send_str(" ");
	uart_send_str("\nADDER:");
	uart_putchar(ADDER);
	uart_send_str("\nMEM   ");
	uart_send_str("\n   ");
	for (i = 0; i < COLS; i++)
	{
		if (i < 10)
		{
			uart_send_str("  ");
		}
		else uart_send_str("  ");
		uart_putchar(i);
		uart_send_str(" ");
	}
	for (i = 0; i < MAX_MEM; i++)
	{
		if (i % COLS == 0)
		{
			uart_send_str(" ");
			if (i / COLS < 10)uart_send_str(" ");
			uart_putchar(i / COLS);
		}
		uart_send_str("   ");
		uart_putchar( MEM[i]);
		uart_send_str(" ");
	}
	uart_send_str("\n");
}
void runSML() //
{
	int operand = 0, pcode = 0, pstack = 0;
	while (pcode < MAX_MEM)
	{
		int op = MEM[pcode] / 100;    //xxbb
		operand = MEM[pcode] % 100;
		switch (op)
		{
			case INPUT:uart_send_str("INPUT:");
			MEM[operand]=Scanf_data();
			uart_putchar(MEM[operand]);
			break;
			case PRINT:uart_putchar( MEM[operand]);
			break;
			case LOAD:ADDER = MEM[operand];
			break;
			case STORE:MEM[operand] = ADDER;
			break;
			case SET:ADDER = operand;
			break;
			case ADD:ADDER += MEM[operand];
			break;
			case SUB:ADDER -= MEM[operand];
			break;
			case MUL:ADDER *= MEM[operand];
			break;
			case DIV:ADDER /= MEM[operand];
			break;
			case MOD:ADDER %= MEM[operand];
			break;
			case INC:MEM[operand]++;
			break;
			case DEC:MEM[operand]--;
			break;
			case NEG:MEM[operand] = -MEM[operand];
			break;
			case JMP:pcode = operand - 1;
			break;
			case JMPN:ADDER < 0 ? pcode = operand - 1 : 0;
			break;
			case JMPZ:ADDER == 0 ? pcode = operand - 1 : 0;
			break;
			case AND:ADDER = ADDER & MEM[operand];
			break;
			case OR:ADDER = ADDER | MEM[operand];
			break;
			case XOR:ADDER = ADDER ^ MEM[operand];
			break;
			case PUSH:pstack < MAX_STACK ? STACK[pstack++] = ADDER : uart_send_str("STACK OVERFLOW.");
			break;
			case POP:pstack > 0 ? ADDER = STACK[--pstack] : uart_send_str("STACK EMPTY.");
			break;
			case HALT: pcode = MAX_MEM;
			break;
			default:break;
		}
		pcode++;
	}
	uart_send_str("HALT.");
	dump();
}
uint16_t input_code()
{
	uart_send_str("\nEnter -1 to end input.\n\n");
	uint8_t ct = 0, li = 0;
	memset(MEM, 0, sizeof(int) * MAX_MEM);
	while (1)
	{
		uart_putchar(li);
		ct = Scanf_data();
		if (ct == -1)
		{
			break;
		}
		MEM[li++] = ct;
	}
	return li - 1;
}



uint16_t Scanf_data()
{ 
	uint8_t i;
	uint16_t ct = 0;
	while (1)
	{
		
		i = 0;

		while (uart_char_waiting() > 0 && i < 2)
		{
			ct = (ct << 8) + uart_getchar();
			_delay_ms(2);
			i++;
		}
		if (ct > 0)return ct;
	}
}