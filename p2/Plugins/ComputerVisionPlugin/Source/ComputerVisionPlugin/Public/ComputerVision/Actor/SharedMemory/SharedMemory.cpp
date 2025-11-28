#include "SharedMemory.h"

#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <cstring>
#include <iostream>

SharedMemory::SharedMemory(){

}

SharedMemory::~SharedMemory(){

}

void SharedMemory::Launch(int bytes){
    const char* name = "/my_shm";
    const size_t SIZE = bytes; // 1024;

    // 1. Shared Memory anlegen oder öffnen
    int fd = shm_open(name, O_CREAT | O_RDWR, 0666);
    if (fd == -1) {
        perror("shm_open");
        return;
    }

    // 2. Größe festlegen (ftruncate MUSS bei neuem Segment aufgerufen werden)
    if (ftruncate(fd, SIZE) == -1) {
        perror("ftruncate");
        return;
    }

    // 3. Shared Memory in Prozess einbinden (Mapping)
    void* ptr = mmap(nullptr, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (ptr == MAP_FAILED) {
        perror("mmap");
        return;
    }

    // Benutze es als char*
    char* mem = static_cast<char*>(ptr);

    // 4. Schreiben
    const char* msg = "Hallo Shared Memory";
    std::memcpy(mem, msg, strlen(msg) + 1);

    // 5. Lesen
    std::cout << "Gelesen: " << mem << std::endl;

    // 6. Aufräumen
    munmap(ptr, SIZE);
    close(fd);
    // Wenn du das Segment *löschen* willst:
    // shm_unlink(name);

    //return 0;
}