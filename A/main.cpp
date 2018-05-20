#include <bits/stdc++.h>
#include <SDL.h>
#include <SDL_ttf.h>
#define MaxSpices 6

using namespace std;

const int SCREEN_WIDTH = 450;
const int SCREEN_HEIGHT = 750;

SDL_Window* Window = NULL;
SDL_Surface* gScreen = NULL;
SDL_Surface* wallpaper = NULL;
SDL_Surface* background = NULL;
SDL_Surface* gIMG[6];
SDL_Renderer* renderer;
SDL_Texture* texture[8];
SDL_Rect sourceRect[8];
SDL_Rect desRect[8];
TTF_Font* font1 = NULL;
TTF_Font* font2 = NULL;
TTF_Font* font3 = NULL;

bool xDirection = true;
int Score = 0;
int BestScore = 0;
int Gamesplayed = 0;
int yLimitSpeed = 13;
int xSpeed = 8;
int SpikeL[MaxSpices+2], SpikeR[MaxSpices+2];

bool init()
{
	if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		printf( "ERROR!!!\n");
        return false;
	}
	if (TTF_Init() < 0)
    {
        printf( "ERROR!!!\n");
        return false;
    }
    Window = SDL_CreateWindow( "Dont't Touch The Spikes", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
    if( Window == NULL )
    {
        printf( "ERROR!!!\n");
        return false;
    }
    gScreen = SDL_GetWindowSurface(Window);
    return true;
}


SDL_Surface* loadIMG(string path)
{
    SDL_Surface* loading = SDL_LoadBMP(path.c_str());
    if( loading == NULL ) printf("Error: \n");
    return loading;
}

bool loadMedia()
{
    wallpaper = loadIMG("IMG/wallpaper.bmp");
    if (wallpaper == NULL)
    {
        printf("Error!!!");
        return 0;
    }
    background = loadIMG("IMG/Background.bmp");
    if (background == NULL)
    {
        printf("Error!!!");
        return 0;
    }
    gIMG[0] = loadIMG("IMG/b1.bmp");
    if (gIMG[0] == NULL)
    {
        printf("Error!!!");
        return 0;
    }
    gIMG[1] = loadIMG("IMG/b2.bmp");
    if (gIMG[1] == NULL)
    {
        printf("Error!!!");
        return 0;
    }
    gIMG[2] = loadIMG("IMG/c1.bmp");
    if (gIMG[2] == NULL)
    {
        printf("Error!!!");
        return 0;
    }
    gIMG[3] = loadIMG("IMG/c2.bmp");
    if (gIMG[3] == NULL)
    {
        printf("Error!!!");
        return 0;
    }
    gIMG[4] = loadIMG("IMG/d1.bmp");
    if (gIMG[4] == NULL)
    {
        printf("Error!!!");
        return 0;
    }
    gIMG[5] = loadIMG("IMG/d2.bmp");
    if (gIMG[5] == NULL)
    {
        printf("Error!!!");
        return 0;
    }
    return 1;
}


void close()
{
    SDL_FreeSurface(wallpaper);
    wallpaper = NULL;
    SDL_DestroyWindow(Window);
    Window = NULL;
    TTF_Quit();
    SDL_Quit();
}

void Prepare()
{
    renderer = SDL_CreateRenderer(Window, -1, SDL_RENDERER_ACCELERATED);
    for(int i = 0; i < 6; i++)
    {
        texture[i] = SDL_CreateTextureFromSurface(renderer, gIMG[i]);
        SDL_QueryTexture(texture[i], NULL, NULL, &sourceRect[i].w, &sourceRect[i].h);
        sourceRect[i].x = 0;
        sourceRect[i].y = 0;
        desRect[i].x = 0;
        desRect[i].y = 0;
        desRect[i].w = sourceRect[i].w;
        desRect[i].h = sourceRect[i].h;
    }
    texture[6] = SDL_CreateTextureFromSurface(renderer, wallpaper);
    SDL_QueryTexture(texture[6], NULL, NULL, &sourceRect[6].w, &sourceRect[6].h);
    sourceRect[6].x = 0;
    sourceRect[6].y = 0;
    desRect[6].x = 0;
    desRect[6].y = 0;
    desRect[6].w = sourceRect[6].w;
    desRect[6].h = sourceRect[6].h;
    // background
    texture[7] = SDL_CreateTextureFromSurface(renderer, background);
    SDL_QueryTexture(texture[7], NULL, NULL, &sourceRect[7].w, &sourceRect[7].h);
    sourceRect[7].x = 0;
    sourceRect[7].y = 0;
    desRect[7].x = 0;
    desRect[7].y = 0;
    desRect[7].w = sourceRect[7].w;
    desRect[7].h = sourceRect[7].h;
}

bool loadFont()
{
    font1 = TTF_OpenFont("D:/Games/A/Font/stocky.ttf", 130);
    font2 = TTF_OpenFont("D:/Games/A/Font/Roboto-Black.ttf", 25);
    font3 = TTF_OpenFont("D:/Games/A/Font/Roboto-Black.ttf", 15);
    if (font1 == NULL || font2 == NULL || font3 == NULL)
    {
        cout << "Can't load Font!!!";
        return 0;
    }
    return 1;
}

void WriteScore(int x, int y, string s)
{
    //SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_Color fg = { 235, 235, 235 };
    SDL_Surface* TempSurface = TTF_RenderText_Solid(font1, s.c_str(), fg);
	SDL_Texture* ScoreText = NULL;
	ScoreText = SDL_CreateTextureFromSurface(renderer, TempSurface);
	SDL_FreeSurface(TempSurface);
	SDL_Rect srcRest;
	SDL_Rect desRect;
	TTF_SizeText(font1, s.c_str(), &srcRest.w, &srcRest.h);
	srcRest.x = 0;
	srcRest.y =  0;
	desRect.x = x;
	desRect.y = y;
	desRect.w = srcRest.w;
	desRect.h = srcRest.h;
    SDL_RenderCopy(renderer, ScoreText, &srcRest, &desRect);
    ScoreText = NULL;
    //SDL_RenderPresent(renderer);
}

void WriteBestScore(int x, int y, string s)
{
    //SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_Color fg = { 123, 123, 123 };
    SDL_Surface* TempSurface = TTF_RenderText_Solid(font2, s.c_str(), fg);
	SDL_Texture* ScoreText = NULL;
	ScoreText = SDL_CreateTextureFromSurface(renderer, TempSurface);
	SDL_FreeSurface(TempSurface);
	SDL_Rect srcRest;
	SDL_Rect desRect;
	TTF_SizeText(font2, s.c_str(), &srcRest.w, &srcRest.h);
	srcRest.x = 0;
	srcRest.y =  0;
	desRect.x = x;
	desRect.y = y;
	desRect.w = srcRest.w;
	desRect.h = srcRest.h;
    SDL_RenderCopy(renderer, ScoreText, &srcRest, &desRect);
    ScoreText = NULL;
    //SDL_RenderPresent(renderer);
}

void Write(int x, int y, string s)
{
    //SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_Color fg = { 238, 42, 75 };
    SDL_Surface* TempSurface = TTF_RenderText_Solid(font3, s.c_str(), fg);
	SDL_Texture* ScoreText = NULL;
	ScoreText = SDL_CreateTextureFromSurface(renderer, TempSurface);
	SDL_FreeSurface(TempSurface);
	SDL_Rect srcRest;
	SDL_Rect desRect;
	TTF_SizeText(font3, s.c_str(), &srcRest.w, &srcRest.h);
	srcRest.x = 0;
	srcRest.y =  0;
	desRect.x = x;
	desRect.y = y;
	desRect.w = srcRest.w;
	desRect.h = srcRest.h;
    SDL_RenderCopy(renderer, ScoreText, &srcRest, &desRect);
    ScoreText = NULL;
    //SDL_RenderPresent(renderer);
}


void Memset()
{
    for(int i = 0; i < 6; i++)
    {
        sourceRect[i].x = 0;
        sourceRect[i].y = 0;
        desRect[i].x = 0;
        desRect[i].y = 0;
        desRect[i].w = sourceRect[i].w;
        desRect[i].h = sourceRect[i].h;
    }
    sourceRect[6].x = 0;
    sourceRect[6].y = 0;
    desRect[6].x = 0;
    desRect[6].y = 0;
    desRect[6].w = sourceRect[6].w;
    desRect[6].h = sourceRect[6].h;
    xDirection = true;
    Score = 0;
}

void ChangeSpeed(int &valY, bool check)
{
    valY++;
    if (check) valY = -yLimitSpeed;
    if (valY > yLimitSpeed) valY--;
}

int Rand(int x, int y)
{
    return rand()%(y-x+1) + x;
}

void MakeSpices()
{
    int cnt = 0, dem = 0, cur = 70;
    while (cnt < MaxSpices)
    {
        int x = Rand(cur, 570);
        int num = 0;
        do num = rand()%3 + 1; while (num + cnt > MaxSpices);
        if (x + 42 * num - 1 < 570)
        {
            for(int i = 0; i < num; i++)
            {
                SpikeL[cnt] = x;
                SpikeR[cnt++] = x + 42 - 1;
                x += 42;
            }
            cur = x + 20;
            dem = 0;
        }
        else dem++;
        if (dem == 5 && cnt < MaxSpices)
        {
            SpikeL[cnt] = -1;
            SpikeR[cnt] = -1;
            break;
        }
    }
}

bool InRange(int y)
{
    for(int i = 0; i < MaxSpices; i++)
    {
        if (SpikeL[i] == -1) break;
        int l = y, r = y + 40 - 1;
        if (!(r <= SpikeL[i] || l >= SpikeR[i])) return true;
    }
    return false;
}

bool CheckAlive(int &x, int &y, bool Direction)
{
    bool ok = true;
    if (y <= 63)
    {
        y = 63;
        ok = false;
    }
    if (y >= 548)
    {
        y = 548;
        ok = false;
    }
    if (Direction == true)
    {
        if (x >= 334)
        {
            if (InRange(y))
            {
                ok = false;
                x = 336;//334;
            }
        }
    }
    else
    {
        if (x <= 56)
        {
            if (InRange(y))
            {
                ok = false;
                x = 54;//56;
            }
        }
    }
    return ok;
}

string str(int x)
{
    stringstream ss;
    ss << x;
    string s; ss >> s;
    return s;
}


void Died(bool xDirection)
{
    int temp = 5 - xDirection, d = 0;
    int y = desRect[0].y;
    if (y >= 548) d = 1;
    int valx = 1;
    if (desRect[0].x > 225) valx = -valx;
    if (d == 0)
    {
        int i = 0;
        while (true)
        {
            string result = str(Score);
            SDL_RenderCopy(renderer, texture[6], &sourceRect[6], &desRect[6]);
            if (Score < 10) result = "0" + result;
            WriteScore(150,270,result);
            i++;
            SDL_RenderCopyEx(renderer, texture[temp],  &sourceRect[temp], &desRect[0], 1.0 * (i%360), NULL,SDL_FLIP_NONE);
            if (i%10 == 0 && i > 0)
            {
                desRect[0].x += valx;
                if (i >= 40) desRect[0].y += 5;
            }
            if (desRect[0].y >= 548) break;
            if (desRect[0].x <= 40 || desRect[0].x >= 360) break;
            SDL_RenderPresent(renderer);
            SDL_Delay(1);
            SDL_RenderClear(renderer);
        }
    }
    else
    {
        int i = 0;
        while (true)
        {
            i++;
            SDL_RenderCopy(renderer, texture[6], &sourceRect[6], &desRect[6]);
            SDL_RenderCopyEx(renderer, texture[temp],  &sourceRect[temp], &desRect[0], 1.0 * (i%360), NULL,SDL_FLIP_NONE);
            if (i%10 == 0 && i > 0)
            {
                desRect[0].x += valx;
                if (i >= 250) desRect[0].y += 5;
                else if (i >= 150) desRect[0].y += 2;
                else desRect[0].y -= 2;
            }
            if (desRect[0].y >= 548 && i >= 250) break;
            if (desRect[0].x <= 40 || desRect[0].x >= 360) break;
            SDL_RenderPresent(renderer);
            SDL_Delay(1);
            SDL_RenderClear(renderer);
        }
    }
}


void SingleGame()
{
    int valX = xSpeed, valY = 0;
    SDL_Event e;
    bool isalive = true;
    desRect[0].x = SCREEN_WIDTH/2-30;
    desRect[0].y = SCREEN_HEIGHT/2-52;
    int EPS = 20;
    //MakeSpices();
    SpikeL[0] = -1;
    SpikeR[0] = -1;
    while (isalive)
	{
	    bool ok = false;
        if ( SDL_PollEvent( &e ) != 0 )
        {
            if (e.type == SDL_QUIT)
            {
                isalive = false;
                break;
            }
            else if (e.type == SDL_KEYDOWN)
                ok = ((e.key.keysym.sym == SDLK_UP) || (e.key.keysym.sym == SDLK_SPACE));
        }
        // wall
        SDL_RenderCopy(renderer, texture[6], &sourceRect[6], &desRect[6]);
        // score
        string result = str(Score);
        if (Score < 10) result = "0" + result;
        WriteScore(150,270,result);
	    if (xDirection == true)
        {
            ChangeSpeed(valY, ok);
            desRect[0].x += valX;
            desRect[0].y += valY;
            if (!CheckAlive(desRect[0].x, desRect[0].y, xDirection)) isalive = false;
            if (desRect[0].x >= SCREEN_WIDTH - 60 - 30)
            {
                if (isalive) Score++;
                desRect[0].x = SCREEN_WIDTH - 60 - 30;
                valX = -valX;
                xDirection = !xDirection;
                MakeSpices();
            }
            SDL_RenderCopy(renderer, texture[0], &sourceRect[0], &desRect[0]);
        }
        else
        {
            ChangeSpeed(valY, ok);
            desRect[0].x += valX;
            desRect[0].y += valY;
            if (!CheckAlive(desRect[0].x, desRect[0].y, xDirection)) isalive = false;
            if (desRect[0].x <= 30)
            {
                if (isalive) Score++;
                desRect[0].x = 30;
                valX = -valX;
                xDirection = !xDirection;
                MakeSpices();
            }
            SDL_RenderCopy(renderer, texture[1], &sourceRect[1], &desRect[0]);
        }
        // chong
        if (xDirection == true)
        {
            for(int i = 0; i < MaxSpices; i++)
            {
                if (SpikeL[i] == -1) break;
                desRect[3].x = 395;
                desRect[3].y = SpikeL[i];
                SDL_RenderCopy(renderer, texture[3], &sourceRect[3], &desRect[3]);
            }
        }
        else
        {
            for(int i = 0; i < MaxSpices; i++)
            {
                if (SpikeL[i] == -1) break;
                desRect[2].x = 30;
                desRect[2].y = SpikeL[i];
                SDL_RenderCopy(renderer, texture[2], &sourceRect[2], &desRect[2]);
            }
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(EPS);
        if (!isalive)
        {
            Died(xDirection);
            BestScore = max(BestScore, Score);
            Gamesplayed++;
        }
        SDL_RenderClear(renderer);
	}
}

int main( int argc, char* args[] )
{
    srand(time(NULL));
	if(!init() || !loadMedia() || !loadFont())
	{
		printf("ERROR!!!");
		return 0;
	}
    Prepare();
    SDL_Event e;
    bool isRun = true;
    while (isRun)
    {
        SDL_RenderCopy(renderer, texture[7], &sourceRect[7], &desRect[7]);
        if (Gamesplayed <= 1)
        {
            Write(110, 210, "Press button S to start a new game");
            Write(130, 235, "Press button SPACE to jump");
        }
        string temp = "BEST SCORE: " + str(BestScore);
        WriteBestScore(140, 515, temp);
        temp = "GAMES PLAYED: " + str(Gamesplayed);
        WriteBestScore(120, 550, temp);
        SDL_RenderPresent(renderer);
        SDL_RenderClear(renderer);
        if ( SDL_PollEvent( &e ) != 0 )
        {
            if (e.type == SDL_QUIT)
            {
                isRun = false;
                break;
            }
            else if (e.type == SDL_KEYDOWN)
            {
                if ((e.key.keysym.sym) == SDLK_s)
                {
                    SingleGame();
                    Memset();
                }
                else if ((e.key.keysym.sym) == SDLK_ESCAPE)
                {
                    isRun = false;
                    break;
                }
            }
        }
    }
	close();
	return 0;
}
