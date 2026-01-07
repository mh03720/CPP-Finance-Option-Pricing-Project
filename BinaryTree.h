#pragma once
#include <vector>
#include <iostream>
#include <iomanip>
#include <sstream>

// Classe template représentant un arbre binaire sous forme triangulaire. Le noeud (n,i) correspond à la ligne n et à la position i (0 <= i <= n).
template<typename T>
class BinaryTree {
private:
	int _depth; // Profondeur de l'arbre
	std::vector<std::vector<T>> _tree; // Structure triangulaire stockant les valeurs des noeuds

public:
	// Constructeur par défaut : arbre vide de profondeur 0
	BinaryTree() : _depth(0) {}

	// Initialise la profondeur de l'arbre et alloue la structure triangulaire. La ligne n contient exactement n+1 noeuds.
	void setDepth(int depth) {
		_depth = depth;
		_tree.clear();
		_tree.resize(depth + 1);

		for (int n = 0; n <= depth; ++n) {
			_tree[n].resize(n + 1);
		}
	}

	// Affecte la valeur du noeud (n,i). Des vérifications sont effectuées pour éviter les accès invalides.
	void setNode(int n, int i, const T& value) {
				if (n < 0 || n > _depth || i < 0 || i > n) {
			throw std::out_of_range("Invalid node indices");
		}
		_tree[n][i] = value;
	}

	// Retourne la valeur stockée au noeud (n,i). Les indices sont vérifiés afin d'assurer la sécurité
	T getNode(int n, int i) const {
		if (n < 0 || n > _depth || i < 0 || i > n) {
			throw std::out_of_range("Invalid node indices");
		}
		return _tree[n][i];
	}

	// Retourne la profondeur de l'arbre.
	int getDepth() const {
		return _depth;
	}

	/*Affiche le contenu de l'arbre. L'affichage se fait en deux parties :
		-1) une représentation triangulaire type "tableau"
		-2) une représentation graphique en forme d'arbre avec branches "/ \"
	 Cette présentation correspond aux figures fournies dans l'énoncé.*/
	void display() const {
		// Formatage numérique uniforme pour l'affichage
		std::cout << std::fixed << std::setprecision(4);

		//Affichage sous forme triangulaire simple
		for (int n = 0; n <= _depth; ++n) {
			for (int i = 0; i <= n; ++i) {
				std::cout << std::setw(2) << _tree[n][i] << " ";
			}
			std::cout << std::endl;
		}
		std::cout << '\n';
		if (_depth < 0) return;

		//Calcul dynamique de la largeur maximale d'un noeud. Cela permet un affichage centré et lisible, quelle que soit la taille des valeurs.
		int maxValWidth = 1;
		for (int n = 0; n <= _depth; ++n)
			for (int i = 0; i <= n; ++i)
				maxValWidth = std::max<int>(maxValWidth, int(std::to_string(_tree[n][i]).size()));

		// Largeur d'une colonne et espacement entre les noeuds
		const int COL = std::max(3, maxValWidth) + 1;   // largeur d'une colonne
		const int BETWEEN = COL;                        // espacement entre colonnes

		//Affichage en forme d'arbre
		for (int n = 0; n <= _depth; ++n) {
			// Indentation pour centrer la ligne n
			int indentValues = (_depth - n) * (BETWEEN / 2 + COL / 2);
			std::cout << std::string(indentValues, ' ');

			// Affichage des valeurs de la ligne n
			for (int i = 0; i <= n; ++i) {
				std::cout << std::setw(COL) << _tree[n][i];
				if (i < n) std::cout << std::string(BETWEEN, ' ');
			}
			std::cout << '\n';

			// Affichage des branches reliant au niveau suivant
			if (n < _depth) {

				int indentBranches = indentValues + (COL / 2);
				if (indentBranches < 0) indentBranches = 0;
				std::cout << std::string(indentBranches, ' ');

				for (int i = 0; i <= n; ++i) {
					// Représentation des liens entre les noeuds
					std::cout << "/ \\";
					if (i < n) std::cout << std::string(BETWEEN + COL - 3, ' ');
				}
				std::cout << '\n';
			}
		}
	}
	

	
};



