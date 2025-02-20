
#include "affichage.h"
#include "ButtonEvent.h"
#include <time.h>
#include <stdint.h>

bool running = true;

BUFFER buffer;

BITMAPINFO buffer_bitmap_info;

LRESULT CALLBACK window_callback(
	HWND hwnd,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam
) {
	LRESULT result = 0;		//resultat du traitement des user inpute
	switch (uMsg)
	{
	case WM_CLOSE:
	case WM_DESTROY: {
		running = false;

	}break;
	
	case WM_SIZE: {
		RECT rect;
		GetClientRect(hwnd, &rect);
		buffer.width = rect.right - rect.left;
		buffer.height = rect.bottom - rect.top;

		int buffer_size = buffer.width * buffer.height * sizeof(unsigned int);

		if (buffer.memory)
			VirtualFree(buffer.memory, 0, MEM_RELEASE);
		buffer.memory = VirtualAlloc(0, buffer_size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

		buffer_bitmap_info.bmiHeader.biSize = sizeof(buffer_bitmap_info.bmiHeader);
		buffer_bitmap_info.bmiHeader.biWidth = buffer.width;
		buffer_bitmap_info.bmiHeader.biHeight = buffer.height;
		buffer_bitmap_info.bmiHeader.biPlanes = 1;
		buffer_bitmap_info.bmiHeader.biBitCount = 32;
		buffer_bitmap_info.bmiHeader.biCompression = BI_RGB;

	}break;

	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
		break;
	}


	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}


int WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nShowCmd
) {
	//window class
	WNDCLASS window_class = {};
	window_class.style = CS_HREDRAW | CS_VREDRAW;
	window_class.lpszClassName = "Game Window classe";
	window_class.lpfnWndProc = window_callback;

	//regester it 
	RegisterClass(&window_class);
	
	//create window
	HWND window = CreateWindow(window_class.lpszClassName, "game",//<= nom au dessu de la fenetre
		WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, 1940, 1080, 0, 0, hInstance, 0);
	HDC hdc = GetDC(window);

	Image image_copie = cree_image(5, 5);

	Image image = cree_image(5, 5);

	tagRECT rec; rec.left = 100; rec.bottom = 100;

	srand(time(0));

	//------------------INITIALISATION-------------------------//
	for (int j = 1; j < (WINHEIGHT);j++)
		for (int i = 1; i < (WINWIDE);i++) {
			
			if(i%2 ==1)
				image.tab[rand()%1000][rand()%500] = ENVIE;
			
			image.tab[j][i] = MORT;
		}

	Input input = {0};

	//-------------------Debut simulation-----------------------//
	while (running) {

		//inpute
		MSG message;

		for (int i = 0; i < BUTTON_COUNT; i++)
		{
			input.button[i].changed = false;
		}

		while (PeekMessage(&message, window, 0, 0, PM_REMOVE)) {
			TranslateMessage(&message);
			DispatchMessage(&message);
			
			switch (message.message)
			{
			case WM_KEYUP:case WM_KEYDOWN:
			{
				u32 vk_code = (u32)message.wParam;
				bool is_down = ((message.lParam & (1 << 31)) == 0);
				
				switch (vk_code)
				{
				case VK_UP:
				{
					input.button[BUTTON_UP].is_down = is_down;
					input.button[BUTTON_UP].changed = true;
				}break;
				case VK_DOWN:
				{
					input.button[BUTTON_DOWN].is_down = is_down;
					input.button[BUTTON_DOWN].changed = true;
				}break;
				case VK_RIGHT:
				{
					input.button[BUTTON_RIGHT].is_down = is_down;
					input.button[BUTTON_RIGHT].changed = true;
				}break;
				case VK_LEFT:
				{
					input.button[BUTTON_LEFT].is_down = is_down;
					input.button[BUTTON_LEFT].changed = true;
				}break;
				case 0x32: {
					input.button[BUTTON_2].is_down = is_down;
					input.button[BUTTON_2].changed = true;
				}break;
				case 0x33: {
					input.button[BUTTON_3].is_down = is_down;
					input.button[BUTTON_3].changed = true;
				}break;

				default:
					break;
				}
			}
			default:
				break;
			}
		}

		//simulate
		ClearScreen(buffer, MORT);

		//----regle du jeux----//
		for (int j = 1; j<(WINHEIGHT) - 1;j++)
			for (int i = 1; i < (WINHEIGHT) - 1;i++)
			{
				int nb_blanc = verification_nb_blanc(image_copie, i, j);

				if ((nb_blanc > 3 || nb_blanc < 2)&& image_copie.tab[j][i] == ENVIE) {
					image.tab[j][i] = MORT;
				}
				else if(nb_blanc == 3 && image_copie.tab[j][i] == MORT)
				{
					image.tab[j][i] = ENVIE;
				}
			}

		//--------player input---------//

		if (input.button[BUTTON_UP].is_down && rec.bottom + (buffer.height/image.pixheigt) < buffer.height - (3 *(10 / image.pixheigt)) ){
			rec.bottom+= VITTESSE(10/image.pixheigt);
		}
		if (input.button[BUTTON_DOWN].is_down) {
			if(rec.bottom > 3 * (10/image.pixheigt))
				rec.bottom-= VITTESSE(10/image.pixheigt);
		}
		if (input.button[BUTTON_RIGHT].is_down && rec.left + (buffer.width / image.pixwide) < buffer.width- (3 * (10 / image.pixwide))) {
			rec.left+= VITTESSE(10/image.pixheigt);
			
		}
		if (input.button[BUTTON_LEFT].is_down) {
			if (rec.left > 3 *(10 / image.pixheigt))
				rec.left-= VITTESSE(10/image.pixheigt);
		}
		if (input.button[BUTTON_2].is_down)
		{
			image.pixheigt++;
			image.pixwide++;
		}
		if (input.button[BUTTON_3].is_down){
			if (image.pixheigt > 2) {
				image.pixheigt--;
				image.pixwide--;
			}
		}
		

		copierlimage(image, image_copie);
		
		//render
		ImageDansBuffer(image, buffer, rec);

		StretchDIBits(hdc, 0, 0, buffer.width, buffer.height, 0, 0, buffer.width, buffer.height, buffer.memory, &buffer_bitmap_info, DIB_RGB_COLORS, SRCCOPY);
	
		//delay(1000000);
	}
	
}
