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
    return rand();
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
  cout << "validate_grid\n";
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
  cout << "validate_grid\n"; 
  return true;
}
void Sudoku::generate_grid(int k)
{
  cout << "generate_grid\n";
  fill_diags();
  solve_grid();
  remove_elements(k);
  //print_grid(false);
  print_grid(true);
}
void Sudoku::print_grid(bool save)
{
  if (save)
  {
    ofstream fout;
    if(filename.empty()){
        filename = "sud";
    }
    fout.open(filename + "-answer.txt");
    fout<<size<<"\n";
      for (int i = 0; i < size; i++)
      {
        for (int j = 0; j < size; j++)
        {
          fout << grid[i][j] << " ";
        }
        fout << "\n";
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
        if(grid[i][j])
            cout << grid[i][j] << "|";
        else cout<<"*|";
      }
      cout << "\n";
    }
  }
}
void Sudoku::fill_diags()
{
  int block_size = sqrt(size);
  mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
  vector<int> a(size);
  for(int i=1;i<=size;i++) a[i-1] = i;

  for (int blockStart = 0; blockStart < size; blockStart += block_size)
  {
    shuffle(a.begin(),a.end(),rng);
    int cur =0;
    for (int i = blockStart; i < blockStart + block_size; i++)
    {
      for (int j = blockStart; j < blockStart + block_size; j++)
      {
        grid[i][j] = a[cur];
        blocks[{blockStart / block_size, blockStart / block_size}].insert(a[cur]);
        rows[i].insert(a[cur]);
        columns[j].insert(a[cur]);
        cur++;
      }
    }
  }
}
void Sudoku::remove_elements(int k)
{
  vector<pair<int,int>> a;
  a.reserve(size*size);

  for(int i=0;i<size;i++){
      for(int j=0;j<size;j++){
          a.push_back({i,j});
      }
  }
  mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
  shuffle(a.begin(),a.end(),rng);

  for(int i=0;i<k;i++)
  {
      grid[a[i].first][a[i].second] =0;
  }
  cout << "remove_elements\n";
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
  grid.assign(size, vector<int>(size));
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
  if (row >= size)
    return true;
  
  if (col >= size)
  {
    return bt_solve(row + 1, 0);
  }
  // if cell's value is already given, return
  if (grid[row][col])
    return bt_solve(row, col + 1);
  // yay, the row is done! Let's proceed to the next

  mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
  vector<int> a(size);
  for(int i=1;i<=size;i++) a[i-1] = i;

  for (int i = 0; i < size; i++)
  {
    int candidate = a[i];
    if (validate(row, col, candidate))
    {
      // insert candidate
      //  *(*(grid + row) + col) = candidate;
      int temp = (int)sqrt(size);
      pair<int, int> block = make_pair(row / temp, col / temp);

      grid[row][col] = candidate;
      rows[row].insert(candidate);
      columns[col].insert(candidate);
      blocks[block].insert(candidate);

      if (bt_solve(row, col + 1))
        return true;
      rows[row].erase(candidate);
      columns[col].erase(candidate);
      blocks[block].erase(candidate);
      grid[row][col] = 0;
    }
  }
  return false;
}
void Sudoku::solve_grid()
{
  bt_solve(0, 0);
  cout << "grid solved\n";
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