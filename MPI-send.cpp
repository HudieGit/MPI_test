#include <mpi.h>
#include <cstdio>
#include <iostream>

int main(int argc, char** argv){
    MPI_Init(&argc, &argv);

    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    //Складываемое число
    const long long N = 100'000'000'0;
    //
    const int TAG_PARTIAL = 1;

    //Замер времени, начало времени t0
    double t0 = MPI_Wtime();

    //каждый кусок берет свой чанк [bigin, end)
    long long chunk = N / size;
    long long begin = rank * chunk;
    long long end = (rank == size - 1) ? N : begin + chunk;

    //в partial будет посчитана часть суммы каждого чанка, который веделили каждому процу
    long long partial = 0;
    for(long long i = begin; i < end; ++i) partial += i;

    //все  кроме первого процесса отправляют данные
    if(rank != 0) {
        //отправляем данные, первые три в rank 0, с тегом Partial и comm_world. recv должен будет принять именной такой конверт
        MPI_Send(&partial, 1, MPI_LONG_LONG, 0, TAG_PARTIAL, MPI_COMM_WORLD);
    }
    //то есть rang = 0
    else{
        //total - общая суммая, сюда будет складывать
        long long total = partial;
        for(int k = 1; k < size; ++k){
            //сюда вернется сумма одного из процесса
            long long value;
            //какой статус получим от процесса, из него сможем узнать, какой процесс мы получили
            MPI_Status status;
            //Первые три значения - это про данные, следующие четыре - конверт, для точного определения, должен совпадать comm, tag, и отправленны в данном случае в 0 процесс
            MPI_Recv(&value, 1, MPI_LONG_LONG, MPI_ANY_SOURCE, TAG_PARTIAL, MPI_COMM_WORLD, &status);
            std:: cout << "get " << value << " from " << status.MPI_SOURCE << std::endl;
            //складываем
            total += value;
        }
        //выводим ожидаемые значения и получившиеся с замером времени работы
        std::cout << "sum = " << total << " expected(" << N * (N - 1) / 2 << "), time " << MPI_Wtime() - t0 << std::endl;
    }

    MPI_Finalize();
    return 0;
}