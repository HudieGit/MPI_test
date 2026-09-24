#include <mpi.h>
#include <cstdio>
#include <iostream>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    //уже с этого момента есть множество процессы
    //size - колисчество создоваемых процессов(OUT)
    //rank - номер процесса (OUT)
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    //std::cout << "size: " << size << '\n' << "rank: "<< rank << '\n';
    //Кажый процесс вернет кол-во всех процессов(size) и свой ранг (0-3)

    //Аналогичный вывод, каждый процес, вернет в разное время, и вывод всегда разный
    //std::cout << "Process " << rank << "of " << size << std::endl;

    char name[MPI_MAX_PROCESSOR_NAME];
    int len;
    //оба параметра OUT, name - имя. len - длина записи имени
    MPI_Get_processor_name(name, &len);
    
    std::cout << "Process " << rank << " of " << size << " on node " << name << std::endl;

    MPI_Finalize();
    return 0;
}
