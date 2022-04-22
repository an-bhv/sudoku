#include <bits/stdc++.h>
using namespace std;

map<pair<int,int>,unordered_set<int>> blocks;
int grid[9][9];
int k;

void solve_grid();
int random_gen(){

    srand(time(0));
    int n= rand()%9;
    n++;
    return n;
}

void fill_diags(){

    for(int b=0;b<9;b+=3){

        for(int i=b;i<b+3;i++){

            for(int j=b;j<b+3;j++){
                
                int n =1;

                do{
                    n = random_gen();
                }
                while(!blocks[{b,b}].count(n));
                
                grid[i][j] = n;
                blocks[{b,b}].insert(n);

            }

        }

    }

}


void remove_elements(){

    int cnt =k;

    while(cnt){

        int i= random_gen()-1;
        int j= random_gen()-1;

        if(grid[i][j]){
            grid[i][j] = 0;
            cnt--;
        }
        
    }

}



void print_grid(bool save){

    if(save){

        ofstream fout;

        fout.open("sudoku.txt");

        while(fout){

            for(int i=0;i<9;i++){
                fout<<"|";
                for(int j=0;j<9;j++){
                    fout << grid[i][j]<<"|";
                }
                fout<<"\n";
            }

        }

        fout.close();

    }

    else{
        for(int i=0;i<9;i++){
                cout<<"|";
                for(int j=0;j<9;j++){
                    cout << grid[i][j]<<"|";
                }
                cout<<"\n";
            }
    }


}



void generate_grid(){

    fill_diags();
    solve_grid();
    remove_elements();
    print_grid(false);

}




