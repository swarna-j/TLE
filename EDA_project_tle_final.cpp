#include <iostream>
#include <stdio.h>
#include <math.h>

#define SIZE_INPUT 100
#define SIZE 50

void sort(int *, int);
void max_freq(int *, int, int &, int &, int &);

using namespace std;

int main()
{
  int a[100], w[50][50] = {0}, d[20][20], t[20], output_seq[20];

  int n, i, j = 0, k = 1, l, size_depthMatrix[20] = {0};
  int flag = 0, x;

  printf("Enter number of inputs\n");
  scanf("%d", &n);
  printf("Enter inputs\n");
  for (i = 0; i < n; i++)
  {
    scanf("%d", &a[i]);
  }

  int seq_size = a[n - 1] + 1;
  int input_seq[seq_size];
  for (i = 0; i < seq_size; i++)
  {
    input_seq[i] = i;
    printf("Input_seq[%d]=%d\n", i, input_seq[i]);
  }

  t[0] = a[0];
  x = a[0];
  int sum_depth = 0;
  for (i = 1; i < n; i++)
  {
    if ((a[i] - a[i - 1]) > 1)
    {
      size_depthMatrix[0]++;
    }
  }
  printf("size of depth matrix %d \n", size_depthMatrix[0]);
  int num_tle = ceil(log2(size_depthMatrix[0] + 1)) + 1;
  printf("Number of Threshold Elements = %d\n", num_tle);
  int tp_x[20][20] = {0}, tp_y_upper[20][20] = {0}, tp_y_lower[20][20] = {0}, fp[20][20] = {0};

  for (i = 1; i < n && j < size_depthMatrix[0]; i++)
  {
    if (a[i] - a[i - 1] > 1)
    {
      if (flag == 1) //a[i] - a[i -1]
      {
        d[0][j] = a[i] - x;
      }
      else
      {
        d[0][j] = a[i] - a[i - 1];
      }
      printf("d[%d] = %d\n", j, d[0][j]);
      if (j == 0)
      {
        sum_depth = 0;
      }
      else
      {
        sum_depth += d[0][j - 1];
      }
      tp_x[0][j] = a[i - 1] + 1;
      tp_y_upper[0][j] = tp_x[0][j] - sum_depth;
      tp_y_lower[0][j] = tp_y_upper[0][j] - d[0][j];
      printf("Transition point = %d, (%d,%d), (%d,%d)\n", j, tp_x[0][j], tp_y_upper[0][j], tp_x[0][j], tp_y_lower[0][j]);
      j++;
      flag = 0;
    }
    else if (a[i] - a[i - 1] == 1)
    {
      if (flag == 0)
      {
        x = a[i - 1];
        flag = 1;
      }
    }
  }
  int g;
  int num = num_tle;
  for (g = 0; g < num_tle - 1; g++)
  {
    bool isFirst = false;
    int freq = 1, test, maxfreq = 0, depth_val = 0;
    int temp[size_depthMatrix[g]];
    for (i = 0; i < size_depthMatrix[g]; i++)
    {
      temp[i] = d[g][i];
      printf("temp[%d] = %d\n", i, temp[i]);
    }
    sort(temp, size_depthMatrix[g]);
    max_freq(temp, size_depthMatrix[g], maxfreq, freq, depth_val);
    if (maxfreq == 0)
    {
      depth_val = d[g][0];
    }
    printf("maxfreq = %d\t depth_val = %d\n", maxfreq, depth_val);

    for (i = 0, j = 0; i < size_depthMatrix[g]; i++)
    {
      if (d[g][i] == depth_val)
      {
        tp_x[g + 1][j] = tp_x[g][i + 1];
        tp_y_upper[g + 1][j] = tp_y_upper[g][i + 1] + d[g][i];
        tp_y_lower[g + 1][j] = tp_y_lower[g][i + 1];

        if (isFirst == false)
        {
          t[g + 1] = tp_y_upper[g][j];
          fp[g][j] = tp_x[g][i];
          for (k = 0; k <= g; k++)
          {
            w[num - 1][num_tle - k] = fp[g][j] - tp_y_lower[g][i];
            printf("weight[%d][%d] = %d\n", (num - 1), (num_tle - k), w[num - 1][num_tle - k]);
          }

          printf("folding point [%d][%d] = %d\n", g, j, fp[g][j]);
          printf("threshold[%d] = %d\n", g + 1, t[g + 1]);
          isFirst = true;
        }
        if (i == (size_depthMatrix[g] - 1))
        {
          tp_x[g + 1][j] = tp_x[g][i];
          tp_y_upper[g + 1][j] = tp_y_upper[g][i];
          tp_y_lower[g + 1][j] = tp_y_lower[g][i];
        }
        printf("Transition point = %d, (%d,%d), (%d,%d)\n", j, tp_x[g + 1][j], tp_y_upper[g + 1][j], tp_x[g + 1][j], tp_y_lower[g + 1][j]);
        i++;
      }
      else if (d[g][i] != depth_val)
      {
        tp_x[g + 1][j] = tp_x[g][i];
        tp_y_upper[g + 1][j] = tp_y_upper[g][i];
        tp_y_lower[g + 1][j] = tp_y_lower[g][i];
        printf("Transition point = %d, (%d,%d), (%d,%d)\n", j, tp_x[g + 1][j], tp_y_upper[g + 1][j], tp_x[g + 1][j], tp_y_lower[g + 1][j]);
      }
      else
      {
      }
      d[g + 1][j] = tp_y_upper[g + 1][j] - tp_y_lower[g + 1][j];
      j++;
    }
    size_depthMatrix[g + 1] = j;
    num = num - 1;
  }
  compute(input_seq, seq_size, t, w, num_tle, out_seq);
  return 0;
}

void sort(int *array, int size)
{
  int i, j, test;
  for (i = 0; i < size; i++)
  {
    for (j = i + 1; j < size; j++)
    {
      if (array[i] > array[j])
      {
        test = array[i];
        array[i] = array[j];
        array[j] = test;
      }
    }
  }
}

void max_freq(int *array, int size, int &max_freq, int &freq, int &depth_val)
{
  int i, j;
  for (i = 0, j = 0; i < size; i++)
  {
    if (array[i] == array[i + 1])
    {
      freq++;
      if (freq > max_freq)
      {
        max_freq = freq;
        depth_val = array[i];
      }
    }
    else
    {
      freq = 1;
    }
  }
}

int compute(int inp_seq[], int size, int threshold[], int weight[], int num_tle, int out_seq[])
{
  int x, y, z, m = 0, l;
  int out1[20], out2[20], inp[size];
  for (l = 0; l <= size; l++)
  {
    inp[l] = inp_seq[l];
  }
  for (x = num_tle; x >= 0; x--)
  {
    int i = 0, j = 0, k = 0;

    for (i = 0; i < size; i++)
    {
      if (inp[i] >= threshold[x])
      {
        out1[j] = inp[i];
        printf("out1[%d] = %d\n", j, out1[j]);
        j++;
      }
      else
      {
        out2[k] = inp[i];
        printf("out2[%d] = %d\n", k, out2[k]);
        k++;
      }
    }
    printf("j %d\t, k %d\n", j, k);
    int temp = weight[size_depthMatrix - x];
    printf("weight[%d] %d\n", (size_depthMatrix - x), weight[size_depthMatrix - x]);
    for (y = 0; y < j; y++)
    {
      if ((out1[y] - temp) >= threshold[0])
      {
        out_seq[m] = out1[y];
        printf("out_seq[%d] = %d\n", m, out_seq[m]);
        m++;
      }
    }
    for (z = 0; z < k; z++)
    {
      inp[z] = out2[z];
      printf("input_seq[%d] = %d\n", z, inp[z]);
    }
    size = k;
    printf("size %d\n", size);
  }
  return 0;
}
