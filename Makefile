EXTRACT_SOURCES = ./extract_sources.sh
EXTRACT_USINES = ./extract_usines.sh
AVL_PROGRAM = ./avl.c
HISTO_MAX = ./histo_max.sh
HISTO_REAL = ./histo_real.sh
HISTO_SRC = ./histo_src.sh

all: prepare_data

prepare_data: usines_volumes vol_captation vol_traitement

usines_volumes:
	$(EXTRACT_SOURCES)  # Exécution de extract_sources.sh
	$(EXTRACT_USINES)   # Exécution de extract_usines.sh

vol_captation: usines_volumes
	$(CC) -o avl $(AVL_PROGRAM)  # Compilation de avl.c
	./avl > vol_captation         # Exécution de avl.c pour générer vol_captation

vol_traitement: usines_volumes
	$(CC) -o avl $(AVL_PROGRAM)  # Compilation de avl.c
	./avl > vol_traitement       # Exécution de avl.c pour générer vol_traitement

histo_max: vol_captation
	$(HISTO_MAX) vol_captation   # Exécution de histo_max.sh

histo_real: vol_captation
	$(HISTO_REAL) vol_captation  # Exécution de histo_real.sh

histo_src: vol_traitement
	$(HISTO_SRC) vol_traitement  # Exécution de histo_src.sh

clean:
	rm -f avl vol_captation vol_traitement usines_volumes