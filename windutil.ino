float msToKnots(float ms)
{
  float knots;
  knots = 1.9438461718 * ms;
  return knots;

}

int msToBeaufort(float ms)
{
  int bft;

  if (ms > 0 and ms <= 0.2)
  {
    bft = 0;

  }
  else if (ms > 0.2  and ms <= 1.5)
  {
    bft = 1;
  }

  else if (ms > 1.5  and ms <= 3.3)
  {
    bft = 2;
  }

  else if (ms > 3.3  and ms <= 5.4)
  {
    bft = 3;
  }

  else if (ms > 5.4  and ms <= 7.9)
  {
    bft = 4;
  }

  else if (ms > 7.9  and ms <= 10.7)
  {
    bft = 5;
  }

  else if (ms > 10.7  and ms <= 13.8)
  {
    bft = 6;
  }

  else if (ms > 13.8  and ms <= 17.1)
  {
    bft = 7;
  }

  else if (ms > 17.1  and ms <= 20.7)
  {
    bft = 8;
  }

  else if (ms > 20.7  and ms <= 24.4)
  {
    bft = 9;
  }

  else if (ms > 24.4  and ms <= 28.4)
  {
    bft = 10;
  }

  else if (ms > 28.4  and ms <= 32.6)
  {
    bft = 11;
  }

  else if (ms > 32.6  and ms <= 117)
  {
    bft = 12;
  }


  else
  {
    bft = -1;
    // do Thing C
  }


  return bft;

}



