#include "my_printf.h"
#include <stdarg.h>

int base_converter(int convert, int basee)
{
  char base_digits[16] =
    {'0', '1', '2', '3', '4', '5', '6', '7',
     '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};
  
  int converted_number[64];
  long int number_to_convert = convert;
  int base;
  int index=0;
  base = basee;
  while (number_to_convert != 0)
    {
      converted_number[index] = number_to_convert % base;
      number_to_convert = number_to_convert / base;
      ++index;
    }
  
  --index;  /* back up to last entry in the array */
  while(index >= 0)
    {  
      ft_putchar(base_digits[converted_number[index]]);
      index--;
    }
  return (0);
}

int base_converter_upper(int convert, int basee)
{
  char base_digits[16] =
    {'0', '1', '2', '3', '4', '5', '6', '7',
     '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

  int converted_number[64];
  long int number_to_convert = convert;
  int base;
  int index=0;
  base = basee;
  while (number_to_convert != 0)
    {
      converted_number[index] = number_to_convert % base;
      number_to_convert = number_to_convert / base;
      ++index;
    }

  --index;  /* back up to last entry in the array */
  while(index >= 0)
    {
      ft_putchar(base_digits[converted_number[index]]);
      index--;
    }
  return (0);
}

int manage_arg(t_main *main, va_list *myva)
{
  int nb;
  char *chaine;
  unsigned int number;
  int c;
  void *adress;

  main->i++;
  if(main->chaine[main->i] == 'd' || main->chaine[main->i] == 'i')
    {
      if(main->etat == 0)
	ft_putnbr((int)main->elem);
      else  if(main->etat == 1)
        ft_putnbr((long int)main->elem);
      else  if(main->etat == 2)
        ft_putnbr((long long int)main->elem);
    }
  else if(main->chaine[main->i] == 's')
    {
      ft_putstr((char *)main->elem);
    }
  else if(main->chaine[main->i] == 'u')
    {
      ft_putnbr((unsigned int)main->elem);
    }
  else if(main->chaine[main->i] == 'c')
    {
      c = va_arg(*myva, int);
      ft_putchar((int)main->elem);
    }
  else if(main->chaine[main->i] == '%')
    ft_putchar('%');
  else if(main->chaine[main->i] == 'p')
    {
      adress = va_arg(*myva, void *);
      ft_putstr("0x");
      base_converter((int)adress, 16);
    }
  else if(main->chaine[main->i] == 'x')
    {
      number = va_arg(*myva, unsigned int);
      base_converter(number, 16);
    }
  else if(main->chaine[main->i] == 'X')
    {
      number = va_arg(*myva, unsigned int);
      base_converter_upper(number, 16);
    }
  else if(main->chaine[main->i] == 'o')
    {
      number = va_arg(*myva, unsigned int);
      ft_putnbr(number);
    }
  else
    main->i--;
  return (1);
}

int lenght_fiel(t_main *main)
{
  main->i++;
  if(main->chaine[main->i] == 'l' && main->etat == 0)
    {
      main->etat = 1;
      if(main->chaine[main->i + 1] == 'l')
	{
	  main->i++;
	  main->type = 2;
	}
      else
	main->type = 1;    
      lenght_fiel(main);
    }
  else if(main->chaine[main->i] == 'h' && main->etat == 0)
    {
      main->etat = 1;
      if(main->chaine[main->i + 1] == 'h')
        {
          main->i++;
          main->type = 4;
        }
      else
        main->type = 3;
      lenght_fiel(main);
    }
  else if(main->chaine[main->i] == 'j')
    {
      main->etat = 1;
      main->type = 5;
      lenght_fiel(main);
    }
  else if(main->chaine[main->i] == 'z')
    {
      main->etat = 1;
      main->type = 6;
      lenght_fiel(main);
    }
  else
    main->i--;
  return (0);
}

int flag_fiel(t_main *main)
{
  char *tr;
  int used;
  int needzero;
  int totalneed;

  main->i++;
  if(main->chaine[main->i] == '+')
    {
      if((int)main->elem >= 0)
	{
	  main->taille++;
	  ft_putchar('+');
	}
      flag_fiel(main);
    }
  else if(main->chaine[main->i] == '#')
    {
      //http://alain.dancel.free.fr/c/c60_10.htm
      //voir ce site pour le gerer
      flag_fiel(main);
    }
  else if(main->chaine[main->i] == ' ')
    {
      if((int)main->elem >= 0)
        {
          main->taille++;
          ft_putchar(' ');
        }
      flag_fiel(main);
    }
  else if(main->chaine[main->i] == '0')
    {
      tr = ft_itoa((int)main->elem);
      used = ft_strlen(tr);
      needzero = main->taille + used;
      if(main->chaine[main->i + 1] >= '0' && main->chaine[main->i +1] <= '9')
	{
	  main->i++;
	  totalneed = ft_atoi(&main->chaine[main->i]);
	  if(totalneed > needzero)
	    {
	      while(needzero < totalneed)
		{
		  ft_putchar('0');
		  needzero++;
		}
	    }
	}
      flag_fiel(main);
    }
  else
    main->i--;
  return (0);
}

void parser(t_main *main)
{

  //liste chainer d'element
}

int ft_printf(const char *format, ...)
{
  va_list myva;
  t_main *main;

  main = (t_main *)malloc(sizeof(t_main) * 1); 
  main->taille = 0;
  main->etat = 0;
  main->i = 0;
  main->chaine = format;
  //initializzzzzze VA  
  va_start(myva, format); 
  while(format[main->i] != '\0')
    {
      if(format[main->i] != '%')
	{
	  ft_putchar(format[main->i]);
	  //  main->i++;
	}      
      else
	{
	  main->elem = va_arg(myva, void *); 
	  flag_fiel(main);
	  lenght_fiel(main);
	  manage_arg(main, &myva);
	}
      main->i++;
    }
  //END de mon VA
  va_end(myva); 
 return (1);
}

//SpdDoOuUxXcC
int main()
{
  //les modifier l ll etc voir dans le man 3 printf
  unsigned int a = -3;
  char *b = "salut";
  unsigned int c = 934549;
long int i = -4;
 int y = 2;
 long long int p = 3;

  printf("VRAI PRINTF : \n");
  printf("lol %lld\n", p);
  printf("MON PRINTF : \n");
  ft_printf("lol %lld\n",p);
  return (0);
}
