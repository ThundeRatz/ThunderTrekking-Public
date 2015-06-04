#include "mixagem.h"
#define coorMAX 63                     /*coor max recebido*/
#define coorMIN -63                     /*coor min recebido*/
#define coorZERO (coorMAX+coorMIN)/2    /*Deve ser inteiro*/
#define velMAX 255                      /*vel max enviada*/
#define velMIN 0                        /*vel min enviada*/

int MOD(int x)
{
    if(x > 0)
        return x;
    else return -x;
}

int MAX(int x, int y)
   {
        if(x>y)return x;
        else return y;
   }

uint16_t mixagem(int8_t coorX, int8_t coorY)
{
    static int m[2];

    int MODULO = (int)(MAX(MOD(coorX), MOD(coorY)));

    if (MODULO < 10)
        return 0;
    /*m[0] = velocidade no motor esquerdo
      m[1] = velocidade no motor direito


      Dependem da distância à origem:
        Quadrantes ímpares: left
        Quadrante    pares: right
      Quadrante 1:
        right aumenta
      Quadrante 2:
        left diminui
      Quadrante 3:
        right diminui
      Quadrante 4:
        left aumenta
    */


    if(coorY > coorZERO) /*1К ou 2К quadrantes*/
    {
        if(coorX > coorZERO) /*1К quadrante*/
        {
           m[0] = MODULO;
           m[1] = coorY - coorX;
        }
        else              /*2К quadrante*/
        {
            m[0] = coorY + coorX;
            m[1] = MODULO;
        }
    }
    else /*3К ou 4К quadrantes*/
    {
        if(coorX < coorZERO) /*3К quadrante*/
        {
            m[0] = -MODULO;
            m[1] = coorY - coorX;
        }
        else /*4К quadrante*/
        {
            m[0] = coorY + coorX;
            m[1] = -MODULO;
        }
    }

    if(m[0] > 63)
        m[0] = 63;
    if(m[0] < -63)
        m[0] = -63;
    if(m[1] > 63)
        m[1] = 63;
    if(m[1] < -63)
        m[1] = -63;
    return (m[0] << 8) | (m[1] & 0xff);
}
