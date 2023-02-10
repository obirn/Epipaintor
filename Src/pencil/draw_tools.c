#include "draw_tools.h"
#include "pixel_operations.h"

//This function create a cross of pixel with the size indicated
// -> 'surface' the pointer on SDL_Surface
// -> 'color' the color of the point (the color can be transparent to be a rubber)
// -> 'x' and 'y' coordinates where the point shall be
// -> 'size' the size of the cross
void point(SDL_Surface* surface, SDL_Color color, int x, int y, int size)
{
    if (surface == NULL)
        return;
    //if (x < 0 || y < 0 || x >= surface->w || y >= surface->h)
    //    return;

    int i = x-size;
    int j = y;
    int nb = 0;
    while(i < x){
	    while(j < y+nb){
            if (i >= 0 && j >= 0 && i < surface->w && j < surface->h)
		        put_pixel(surface, i, j, SDL_MapRGB(surface->format, color.r, color.g, color.b));
		    j++;
	    }
	    i++;
	    nb++;
	    j = y - nb;
    }
    while(i <= x+size){
            while(j < y+nb){
                if (i >= 0 && j >= 0 && i < surface->w && j < surface->h)
                    put_pixel(surface, i, j, SDL_MapRGB(surface->format, color.r, color.g, color.b));
                j++;
            }
            i++;
            nb --;
            j = y - nb;
    }
}

//Draw a line of point() between the two coordinates
// -> 'surface' the pointer on SDL_Surface
// -> 'x1' and 'y1' coordinates of the begin
// -> 'x2' and 'y2' coordinates of the end
// -> 'size' the size of each points
void drawline(SDL_Surface* img, SDL_Color color, int x1, int y1, int x2, int y2, int size)
{
    int x,y;
    int Dx,Dy;
    int xincr,yincr;
    int erreur;
    int i;

    Dx = abs(x2-x1);
    Dy = abs(y2-y1);
    if(x1<x2)
        xincr = 1;
    else
        xincr = -1;
    if(y1<y2)
        yincr = 1;
    else
        yincr = -1;

    x = x1;
    y = y1;
    if(Dx>Dy)
    {
        erreur = Dx/2;
        for(i=0;i<Dx;i++)
        {
            x += xincr;
            erreur += Dy;
            if(erreur>Dx)
            {
                erreur -= Dx;
                y += yincr;
            }
            point(img, color, x, y, size);
        }
    }
    else
    {
        erreur = Dy/2;
        for(i=0;i<Dy;i++)
        {
            y += yincr;
            erreur += Dx;
            if(erreur>Dy)
            {
                erreur -= Dy;
                x += xincr;
            }
            point(img, color, x, y, size);
        }
    }
    point(img, color, x1, y1, size);
    point(img, color, x2, y2, size);
}