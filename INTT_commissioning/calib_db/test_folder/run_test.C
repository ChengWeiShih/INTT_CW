void run_test()
{
  for (int i = 0; i < 20; i++)
    {

      //system(Form("nohup root -l test_1.C&>run_%i.out&",i));
      system("sh run_test.sh");
    }
  //system("nohup root -l test_1.C&&")

}
