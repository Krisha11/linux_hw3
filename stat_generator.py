import subprocess
import matplotlib.pyplot as plt

# build
subprocess.run(["make", "-C", "src"], capture_output=True)


# Для точности статистики имеет смысл делать измерения сразу на большом количестве файлов. Тогда меньшую долю занимают накладные
# затраты не связанные конкретно с чтением и обработкой полученных данных. Кроме того, при увеличении количества файлов - случайные задержки
# которые могут быть получены на конкретном файле вносят меньшую ошибку

# Будем делать измерения на 100 файлах и для разных размеров и разных значений chunk
# Построим три графика: для разных типов времен 
# - внутренний замер
# - утилиты time usr
# - утилиты time sys

def makeMeasure(mode, chunk, readMode, user_times, system_times, times, x, chunks, size) :
    print("measure", chunk, size)
    res = subprocess.run(["head", "-c" + str(size), "/dev/urandom"], capture_output=True)

    f = open("tst.txt", "wb")
    f.write(res.stdout)
    f.close()

    l = ["/usr/bin/time", "src/main", mode, chunk, readMode]
    for _ in range(100) :
        l.append("tst.txt")
    res = subprocess.run(l, capture_output=True)
    time = float(res.stdout.decode("utf-8").split('\n')[1].split(' ')[1])

    time_util_out = res.stderr.decode("utf-8").split('\n')
    user = float(time_util_out[0].split('user')[0])
    system = float(time_util_out[0].split('user ')[1].split('system')[0])

    user_times.append(user)
    system_times.append(system)
    times.append(time)
    x.append(size)
    chunks.append(chunk)

    subprocess.run(["rm", "tst.txt"])


def showGraphicDifSizes(name, mode, chunk, readMode, axs, size_l, size_r) :
    user_times = []
    system_times = []
    times = []
    x = []
    chunks = []

    for size in range(size_l, size_r, (size_r - size_l) // 10 + 1) :
        makeMeasure(mode, chunk, readMode, user_times, system_times, times, x,chunks, size)

    axs.set_title(name, fontsize=15)
    axs.set_xlabel('File size', fontsize=10, color='blue')
    axs.set_ylabel('Time (in sec)', fontsize=10, color='blue')
    axs.legend()
    axs.grid(True)
    axs.plot(x, system_times, label = 'system')
    axs.plot(x, user_times, label = 'user')
    axs.plot(x, times, label = 'full')


def showGraphicDifChunks(name, mode, readMode, size, axs) :
    user_times = []
    system_times = []
    times = []
    x = []
    chunks = []

    for chunk in range(10, size * 2, size // 10) :
        makeMeasure(mode, str(chunk), readMode, user_times, system_times, times, x, chunks, size)

    axs.set_title(name, fontsize=15)
    axs.set_xlabel('Chunk size', fontsize=10, color='blue')
    axs.set_ylabel('Time (in sec)', fontsize=10, color='blue')
    axs.plot(chunks, system_times, label = 'system')
    axs.plot(chunks, user_times, label = 'user')
    axs.plot(chunks, times, label = 'full')
    axs.legend()
    axs.grid(True)


# One byte

fig, axs = plt.subplots(2, 2, figsize=(12, 7))
showGraphicDifSizes('One Byte Low Small', "OneByte", "1", "Low", axs[0, 0], 10, 1024)
showGraphicDifSizes('One Byte Ifstream Small', "OneByte", "1", "Ifstream", axs[0, 1], 10, 1024)
showGraphicDifSizes('One Byte Fread Small', "OneByte", "1", "Fread", axs[1, 0], 10, 1024)

fig, axs = plt.subplots(2, 2, figsize=(12, 7))
showGraphicDifSizes('One Byte Low Medium', "OneByte", "1", "Low", axs[0, 0], 16 * 1024, 128 * 1024)
showGraphicDifSizes('One Byte Ifstream Medium', "OneByte", "1", "Ifstream", axs[0, 1], 16 * 1024, 128 * 1024)
showGraphicDifSizes('One Byte Fread Medium', "OneByte", "1", "Fread", axs[1, 0], 16 * 1024, 128 * 1024)

fig, axs = plt.subplots(2, 2, figsize=(12, 7))
showGraphicDifSizes('One Byte Low Big', "OneByte", "1", "Low", axs[0, 0], 1024 * 1024, 1024 * 1024 + 5)
showGraphicDifSizes('One Byte Ifstream Big', "OneByte", "1", "Ifstream", axs[0, 1], 1024 * 1024, 1024 * 1024 + 5)
showGraphicDifSizes('One Byte Fread Big', "OneByte", "1", "Fread", axs[1, 0], 1024 * 1024, 1024 * 1024 + 5)



# Full

fig, axs = plt.subplots(2, 2, figsize=(12, 7))
showGraphicDifSizes('Full Low Small', "Full", "1", "Low", axs[0, 0], 10, 1024)
showGraphicDifSizes('Full Ifstream Small', "Full", "1", "Ifstream", axs[0, 1], 10, 1024)
showGraphicDifSizes('Full Fread Small', "Full", "1", "Fread", axs[1, 0], 10, 1024)
showGraphicDifSizes('Full Mmap Small', "Full", "1", "Mmap", axs[1, 1], 10, 1024)

fig, axs = plt.subplots(2, 2, figsize=(12, 7))
showGraphicDifSizes('Full Low Medium', "Full", "1", "Low", axs[0, 0], 16 * 1024, 128 * 1024)
showGraphicDifSizes('Full Ifstream Medium', "Full", "1", "Ifstream", axs[0, 1], 16 * 1024, 128 * 1024)
showGraphicDifSizes('Full Fread Medium', "Full", "1", "Fread", axs[1, 0], 16 * 1024, 128 * 1024)
showGraphicDifSizes('Full Mmap Medium', "Full", "1", "Mmap", axs[1, 1], 16 * 1024, 128 * 1024)

fig, axs = plt.subplots(2, 2, figsize=(12, 7))
showGraphicDifSizes('Full Low Big', "Full", "1", "Low", axs[0, 0], 1024 * 1024, 1024 * 1024 + 5)
showGraphicDifSizes('Full Ifstream Big', "Full", "1", "Ifstream", axs[0, 1], 1024 * 1024, 1024 * 1024 + 5)
showGraphicDifSizes('Full Fread Big', "Full", "1", "Fread", axs[1, 0], 1024 * 1024, 1024 * 1024 + 5)
showGraphicDifSizes('Full Mmap Big', "Full", "1", "Mmap", axs[1, 1], 1024 * 1024, 1024 * 1024 + 5)


# Chunk

fig, axs = plt.subplots(2, 2, figsize=(12, 7))
showGraphicDifChunks('Chunk, Small, Low, size = 10', "Chunk", "Low", 10, axs[0, 0])
showGraphicDifChunks('Chunk, Small, Fread, size = 10', "Chunk", "Fread", 10, axs[0, 1])
showGraphicDifChunks('Chunk, Small, Ifstream, size = 10', "Chunk", "Ifstream", 10, axs[1, 0])

fig, axs = plt.subplots(2, 2, figsize=(12, 7))
showGraphicDifChunks('Chunk, Medium, Low, size = 1024 * 32', "Chunk", "Low", 1024 * 16, axs[0, 0])
showGraphicDifChunks('Chunk, Medium, Fread, size = 1024 * 32', "Chunk", "Fread", 1024 * 16, axs[0, 1])
showGraphicDifChunks('Chunk, Medium, Ifstream, size = 1024 * 32', "Chunk", "Ifstream", 1024 * 16, axs[1, 0])

fig, axs = plt.subplots(2, 2, figsize=(12, 7))
showGraphicDifChunks('Chunk, Big, Low, size = 1024 * 1024', "Chunk", "Low", 1024 * 1024, axs[0, 0])
showGraphicDifChunks('Chunk, Big, Fread, size = 1024 * 1024', "Chunk", "Fread", 1024 * 1024, axs[0, 1])
showGraphicDifChunks('Chunk, Big, Ifstream, size = 1024 * 1024', "Chunk", "Ifstream", 1024 * 1024, axs[1, 0])

plt.show()