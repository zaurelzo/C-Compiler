int global;

int fonct(int a , int b )
{
	int h[3]={14,58,63};
	int c =0;
	while(c<3)
	{
		print(h[c]);
		c=c+1;
		
	}

	return 0;

}


main()
{
	int a[3]={1,2,5};
	int c;
	c= fonct(2,5);
	print(c);
	while(c<3)
	{
		print(a[c]);
		c=c+1;
	}
}
