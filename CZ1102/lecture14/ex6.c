void func(int a[], int n)
{
   a[0] += 1;
   ++n;
}
 

main()
{
   int b[2] = {0, 0};
   int n = 0;

   printf("%d %d %d\n", b[0], b[1], n);
   func(b, n);
   printf("%d %d %d\n", b[0], b[1], n);

}

