#include <bits/stdc++.h>
using namespace std;

class Sudoku
{
private:
  int size;
  string filename;
  vector<vector<int>> grid;
  map<int, unordered_set<int>> rows, columns;
  map<pair<int, int>, unordered_set<int>> blocks;
  bool bt_solve(int row, int col);
  int random_gen()
  {
    srand(time(0));
    int n = rand() % size;
    return n + 1;
  }
  void fill_diags();
  void remove_elements(int);

public:
  Sudoku(int);
  Sudoku(string filename);
  bool validate(int row, int col, int val);
  bool validate_grid();
  void solve_grid();
  void generate_grid(int k);
  void print_grid(bool save);
};
bool Sudoku::validate_grid()
{

  int block_size = sqrt(size);
  for (int i = 0; i < size; i++)
  {

    if (rows[i].count(0) || columns[i].count(0))
    {
      return false;
    }

    if (rows[i].size() != size || columns[i].size() != size)
    {
      return false;
    }

    if (i % block_size == 0)
    {
      for (int j = 0; j < block_size; j++)
      {
        if (blocks[{i / block_size, j}].size() != 9)
        {
          return false;
        }
      }
    }
  }

  return true;
}
void Sudoku::generate_grid(int k)
{
  fill_diags();
  solve_grid();
  remove_elements(k);
  // print_grid(false);
}
void Sudoku::print_grid(bool save)
{
  if (save)
  {
    ofstream fout;
    fout.open(filename + "-answer.txt");
    while (fout)
    {
      for (int i = 0; i < size; i++)
      {
        fout << "|";
        for (int j = 0; j < size; j++)
        {
          fout << grid[i][j] << "|";
        }
        fout << "\n";
      }
    }
    fout.close();
  }
  else
  {
    for (int i = 0; i < size; i++)
    {
      cout << "|";
      for (int j = 0; j < size; j++)
      {
        cout << grid[i][j] << "|";
      }
      cout << "\n";
    }
  }
}
void Sudoku::fill_diags()
{
  int block_size = sqrt(size);
  for (int blockStart = 0; blockStart < size; blockStart += block_size)
  {
    for (int i = blockStart; i < blockStart + block_size; i++)
    {
      for (int j = blockStart; j < blockStart + block_size; j++)
      {
        int n = 1;
        do
        {
          n = random_gen();
        } while (!blocks[{blockStart / block_size, blockStart / block_size}].count(n));

        grid[i][j] = n;
        blocks[{blockStart / block_size, blockStart / block_size}].insert(n);
      }
    }
  }
}
void Sudoku::remove_elements(int k)
{
  int cnt = k;
  while (cnt)
  {
    int i = random_gen() - 1;
    int j = random_gen() - 1;

    if (grid[i][j])
    {
      grid[i][j] = 0;
      cnt--;
    }
  }
}

Sudoku::Sudoku(int n)
{
  grid.assign(n, vector<int>(n));
  size = n;
  for (int i = 0; i < n; i++)
  {
    for (int j = 0; j < n; j++)
      grid[i][j] = 0;
  }
}
Sudoku::Sudoku(string filename)
{
  this->filename = filename;
  ifstream inp(filename);
  inp >> size;
  for (int i = 0; i < size; i++)
  {
    for (int j = 0; j < size; j++)
    {
      inp >> grid[i][j];
      if (grid[i][j])
      {
        int row_size = (int)sqrt(size);
        pair<int, int> block = make_pair(i / row_size, j / row_size);

        rows[i].insert(grid[i][j]);
        columns[j].insert(grid[i][j]);
        blocks[block].insert(grid[i][j]);
      }
    }
  }
}
bool Sudoku::validate(int row, int col, int val)
{
  int temp = (int)sqrt(size);
  pair<int, int> block = make_pair(row / temp, col / temp);

  if (!rows[row].empty() && rows[row].find(val) != rows[row].end())
    return false;

  if (!columns[col].empty() && columns[col].find(val) != columns[col].end())
    return false;

  if (!blocks[block].empty() && blocks[block].find(val) != blocks[block].end())
    return false;

  return true;
}
bool Sudoku::bt_solve(int row, int col)
{
  // we are done with all cells, so return
  if (row >= size && col >= size)
    return true;
  // if cell's value is already given, return
  if (grid[row][col])
    return bt_solve(row, col + 1);
  // yay, the row is done! Let's proceed to the next
  if (col >= size)
  {
    return bt_solve(row + 1, 0);
  }
  for (int candidate = 1; candidate <= size; candidate++)
  {
    if (validate(row, col, candidate))
    {
      // insert candidate
      //  *(*(grid + row) + col) = candidate;
      grid[row][col] = candidate;
      if (bt_solve(row, col + 1))
        return true;
      grid[row][col] = 0;
    }
  }
  return false;
}
void Sudoku::solve_grid()
{
  bt_solve(0, 0);
}
void displayOptions();
string options[] = {"Solve", "Generate", "Validate", "Exit"};

int main()
{
  short input;
  cout << "Welcome to Sudoku paradise!\n";
  displayOptions();
  cin >> input;
  while (input != (sizeof(options) / sizeof(options[0])) - 1)
  {
    switch (input)
    {
    case 0:
    {
      cout << "Enter filename: ";
      string filename;
      cin >> filename;
      Sudoku s{filename};
      s.solve_grid();
      s.print_grid(true);
      break;
    }
    case 1:
    {
      int n, k;
      cout << "Enter size of sudoku: ";
      cin >> n;
      cout << "Enter number of blanks that should be present: ";
      cin >> k;
      while (k >= n * n)
      {
        cout << "Funny, but enter something realistic instead: ";
        cin >> k;
      }
      Sudoku s{n};
      s.generate_grid(k);
      s.print_grid(false);
      break;
    }
    case 2:
    {
      cout << "Enter filename: ";
      string filename;
      cin >> filename;
      Sudoku s{filename};
      bool result = s.validate_grid();
      if (result)
        cout << "Sudoku is valid!";
      else
        cout << "Sudoku is invalid :(";
      break;
    }
    default:
      break;
    }
    displayOptions();
    cin >> input;
  }
  return 0;
}
void displayOptions()
{
  cout << endl;
  for (int i = 0; i < sizeof(options) / sizeof(options[0]); i++)
    cout << i << ". " << options[i] << endl;
}