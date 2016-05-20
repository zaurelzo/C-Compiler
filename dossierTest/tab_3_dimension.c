main()
{
	int tab[2][2][2]={{{14,32},{21,74}},{{20,30},{45,98}}};
	
	int i, j, k;
	i=0 ;
	j=0;
	k=0;

	while(i<2)
	{
		while(j<2)
		{
			while(k<2)
			{
				print(tab[i][j][k]);
				k=k+1;
			}
			k=0;
			j=j+1;
		}
		j=0;
		i=i+1;
	}
}
