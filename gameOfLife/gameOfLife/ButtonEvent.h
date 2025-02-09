#pragma once

struct ButtonState
{
	bool is_down;
	bool changed;
};

enum 
{
	BUTTON_UP,
	BUTTON_DOWN,
	BUTTON_LEFT,
	BUTTON_RIGHT,
	BUTTON_2,
	BUTTON_3,
	BUTTON_4,

	BUTTON_COUNT	//laste item de la liste et donne le nombre de bouton 
};

struct Input
{
	ButtonState button[BUTTON_COUNT];
};
