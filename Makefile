CXX = g++
CXXFLAGS = -O3 -std=c++17 -pthread

NAIVE_DIR = naive
FFT_DIR = fft
INPUT_DIR = input

all: fft fft_par naive naive_par naive_bib

# --- FFT SEQUENCIAL ---
fft:
	$(CXX) $(CXXFLAGS) $(FFT_DIR)/fft.cpp -o fft_exec
	@echo "Executando FFT sequencial com entrada $(file)..."
	@./fft_exec < $(INPUT_DIR)/$(file)

# --- FFT PARALELA ---
fft_par:
	$(CXX) $(CXXFLAGS) $(FFT_DIR)/parallel_fft.cpp -o fft_par_exec
	@echo "Executando FFT paralela com entrada $(file)..."
	@./fft_par_exec < $(INPUT_DIR)/$(file)

# --- NAIVE SEQUENCIAL ---
naive:
	$(CXX) $(CXXFLAGS) $(NAIVE_DIR)/naive.cpp -o naive_exec
	@echo "Executando algoritmo ingênuo sequencial com entrada $(file)..."
	@./naive_exec < $(INPUT_DIR)/$(file)

# --- NAIVE PARALELA ---
naive_par:
	$(CXX) $(CXXFLAGS) $(NAIVE_DIR)/parallel_naive.cpp -o naive_par_exec
	@echo "Executando algoritmo ingênuo paralelo com entrada $(file)..."
	@./naive_par_exec < $(INPUT_DIR)/$(file)

# --- NAIVE BIBLIOTECA ---
naive_bib:
	$(CXX) $(CXXFLAGS) $(NAIVE_DIR)/library_naive.cpp -o naive_bib_exec
	@echo "Executando algoritmo ingênuo paralelo biblioteca com entrada $(file)..."
	@./naive_bib_exec < $(INPUT_DIR)/$(file)

# --- LIMPEZA ---
clean:
	rm -f fft_exec fft_par_exec naive_exec naive_par_exec naive_bib_exec

.PHONY: all fft fft_par naive naive_par naive_bib clean