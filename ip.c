#include <stdio.h>
#include <stdlib.h>

#define MAXIM 800
int convert(unsigned int n) //funcita seteaza numarul de biti n pe primele pozitii
{
    unsigned int zecimal=0;
    if(n==0)
        return 0;
    if(n==7)
        return 255;
    while(n)
    {
        zecimal=zecimal | (1<<(n-1));
        n--;
    }
    return zecimal;
}
int compare(unsigned int a,unsigned int b)  //functie care face compararea a doua adrese de forma a.b.c.d
{
    if((a>>24)==(b>>24))
        if(((a<<8)>>24)==(b<<8)>>24)
            if(((a<<16)>>24)==(b<<16)>>24)
                if(((a<<24)>>24)==(b<<24)>>24)
                    return 1;
    return 0;

}
int creare_zecimal(unsigned int n)  //functia creaza forma zecimala a unei masti 
{
    unsigned int a=0,aparitii=0;
    while(n)
    {
        if(n>=8)
        {
            aparitii++;
            switch(aparitii)
            {
            case 1 :
                a=a | (convert(7)<<24);
                break;
            case 2 :
                a=a | (convert(7)<<16);
                break;
            case 3 :
                a=a | (convert(7)<<8);
                break;
            case 4 :
                a=a | (convert(7));
                break;
            }
            n=n-8;

        }
        else
        {
            aparitii++;
            switch(aparitii)
            {
            case 1 :
                a=a | ((convert(n)<<(8-n)<<24));
                break;
            case 2 :
                a=a | ((convert(n)<<(8-n)<<16));
                break;
            case 3 :
                a=a | ((convert(n)<<(8-n)<<8));
                break;
            case 4 :
                a=a | ((convert(n)<<(8-n)));
                break;
            }
            n=0;
        }
    }
    return a;
}
int citire()    //functia citeste datele
{
    unsigned char v[4]= {0};
    unsigned int a=0;
    scanf("%hhu.%hhu.%hhu.%hhu", &v[0],&v[1],&v[2],&v[3]);
    a=a | (v[0]<<24);
    a=a | (v[1]<<16);
    a=a | (v[2]<<8);
    a=a | (v[3]);
    return a;
}
void printare(unsigned int a)   //functia afiseaza datele
{
    printf("%hhu.%hhu.%hhu.%hhu",a>>24,(a<<8)>>24,(a<<16)>>24,(a<<24)>>24);
}
int main()
{
    unsigned char v[4]= {0},MSK_2=0,masca[MAXIM]={0};
    unsigned int MSK_1=0,IP_1=0,IP_2=0;
    unsigned int MSK_2_zecimal=0,NET_[MAXIM]= {0},seturi;
    int i,j,k;
    int N;


    scanf("%d",&seturi);       //citim numarul de seturi de date
    for(k=0; k<seturi; k++)
    {   MSK_1=0;            //initializam datele cu 0 pentru fiecare set de date
        MSK_2=0;
        IP_1=0;
        IP_2=0;
        MSK_2_zecimal=0;
        for(i=0;i<MAXIM;i++){
        NET_[i]=0;
        masca[i]=0;}
        N=0;

        printf("%d\n",k+1);
        MSK_1=MSK_1 | citire(); //facem citirea datelor cu ajutorul functiei
        scanf("%hhu", &MSK_2);
        IP_1=IP_1 | citire();
        IP_2=IP_2 | citire();
        scanf("%d",&N);
        i=j=0;
        while(i<N)          //se face citire pentru cele N adrese de retea
        {
            scanf("%hhu.%hhu.%hhu.%hhu/%hhu",&v[0],&v[1],&v[2],&v[3],&masca[i]);
            NET_[i]=NET_[i] | (v[0]<<24);
            NET_[i]=NET_[i] | (v[1]<<16);
            NET_[i]=NET_[i] | (v[2]<<8);
            NET_[i]=NET_[i] | (v[3]);
            i++;
        }

        printf("-0 ");
        printare(IP_1);
        printf("/%hhu\n",MSK_2);

        MSK_2_zecimal=MSK_2_zecimal | creare_zecimal(MSK_2);    //se creeaza formatul zecimal al MSK_2
        printf("-1 ");
        printare(MSK_2_zecimal);
        printf("\n");
        printf("-2 ");
        printf("%o.%o.%o.%o ",MSK_1>>24,(MSK_1<<8)>>24,(MSK_1<<16)>>24,(MSK_1<<24)>>24);    //se afiseaza in baza 8 msk_1
        printf("%X.%X.%X.%X",MSK_1>>24,(MSK_1<<8)>>24,(MSK_1<<16)>>24,(MSK_1<<24)>>24);     //se afiseaza in baza 16 msk_1
        printf("\n");
        printf("-3 ");
        printare(MSK_2_zecimal &  IP_1);    //se calculeaza si afiseaza adresa de retea a lui IP_1
        printf("\n");
        
        unsigned int complement=0;
        complement=complement | (MSK_2_zecimal<<24);    //se calculeaza complementul mastii MSK_2_zecimal
        complement=complement | (MSK_2_zecimal<<16);
        complement=complement | (MSK_2_zecimal<<8);
        complement=complement | (MSK_2_zecimal);
        printf("-4 ");
        printare(IP_1 | (~complement));     //se calculeaza si afiseaza adresa de broadcas a lui IP_1
        printf("\n");

        printf("-5 ");
        if(compare(MSK_2_zecimal & IP_1,MSK_2_zecimal & IP_2))  //se compara adresele de retea ale lui IP_1 si IP_2
            printf("DA\n");
        else
            printf("NU\n");

        unsigned int gresit=0;
        printf("-6 ");                                      //se face verificarea corectitudinea mastii de retea MSK_1
        for(i=0; i<31 && gresit==0; i++)
            if((MSK_1 & (1<<i))==(1<<i) && (MSK_1&(1<<(i+1)))==0)
                gresit=1;
        if(gresit==0)
        {
            printf("DA\n");             //daca e corecta se afiseaza direct
            printf("-7 ");
            printare(MSK_1);
            printf("\n");
        }
        else
        {
            printf("NU\n"); 
            printf("-7 ");
            unsigned int semnificativ;
            for(i=31; i>=0; i--)        //daca nu, se calculeaza versiunea corecta
            {
                if((MSK_1&(1<<i))==0)
                {
                    semnificativ=i;
                    i=-1;
                }
            }
            MSK_1=0;
            for(i=semnificativ+1; i<32; i++)
                MSK_1=MSK_1 | (1<<i);
            printare(MSK_1);        //se afiseaza versiunea corecta a mastii
            printf("\n");
           

        }

        printf("-8 ");          //se afiseaza in format binar adresa IP_1
        for(i=31; i>=0; i--)
        {
            if(IP_1 & (1<<i))
            {
                printf("1");
            }
            else
            {
                printf("0");
            }
            if(i%8==0 && i!=0)
                printf(".");
        }
        printf("\n");
        printf("-9 ");
        for(i=0; i<N; i++)      
        {
            int  masca2=0;
            masca2=masca2 | creare_zecimal(masca[i]);       //se calculeaza masca de retea pentru fiecare adresa
            if(compare(masca2 & IP_2,masca2 & NET_[i]))     //din cele N citite si se compara adresele de retea
                printf("%d ",i);                        //a lui IP_2 si NET_[i] si in cazul in care corespund
        }                                                  //se afiseaza indicele adresei i;

        printf("\n");
    }

    return 0;
}
