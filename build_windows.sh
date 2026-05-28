make CC=x86_64-w64-mingw32-gcc \
     CXX=x86_64-w64-mingw32-g++ \
     AR=x86_64-w64-mingw32-ar \
     STRIP=x86_64-w64-mingw32-strip \
     WINDRES=x86_64-w64-mingw32-windres clean

export CFLAGS="-Wall -W -Wextra -O0 -g -fsanitize=address"

make CC=x86_64-w64-mingw32-gcc \
     CXX=x86_64-w64-mingw32-g++ \
     AR=x86_64-w64-mingw32-ar \
     STRIP=x86_64-w64-mingw32-strip \
     WINDRES=x86_64-w64-mingw32-windres default -j $((nproc + 1))
