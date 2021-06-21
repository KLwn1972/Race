#pragma once
#include <iostream>
#include <vector>
#include "VectorklassenTest.h"
//#include "Race.h"


using namespace std;

void VectorTest::ProcessingVectorTest() {

	vector<node> nodes;
	node SingleNode;
	std::vector<node>::iterator node_item;
	// Lokalen vector<node> mit den nodes-Informationen des "Klassenobjects" (this, i.e. "TestVector1" aus main).
	nodes = this->nodes;

	// Laenge des Vektors:
	int j = this->nodes.size();


	// Schleife über alle Knoten. node_item ist pointer auf 1.Knoten, Zeiger um ein "node" erhöht mit ++node_item
	int i = -1;
	for (node_item = this->nodes.begin(); node_item != this->nodes.end(); ++node_item) {
		i += 1;
		if (i == 1) {
			//this->nodes.push_back(SingleNode);
		}
		node_item->gradient = i;
		cout << "Gradient von Knoten " << i << " lautet:" << this->nodes[i].gradient << endl;
	}
	for (i = 0; i< this->nodes.size(); ++i) {
			if (i == 1) {
			//this->nodes.push_back(SingleNode);
		}
		this->nodes[i].gradient = i;
		cout << "Gradient von Knoten " << i << " lautet:" << this->nodes[i].gradient << endl;
	}


	cout << "Punkt von lokalem nodes:" << nodes[1].gradient << endl;
	cout << "Punkt aus Klassenobjekt ausgeben:" << this->nodes[7].gradient << endl;
	node_item = this->nodes.begin();
	cout << "1. Punkt aus Klassenobjekt ausgeben:" << node_item->gradient << endl;

	// Einen neuen Knoten am Ende einfügen: einzufügender "node" muss vorhanden sein (initialisiert mit "nan")
	this->nodes.push_back(SingleNode);

	// Einen neuen Knoten am Anfang einfügen: einzufügender "node" muss vorhanden sein (initialisiert mit "nan")
	this->nodes.insert(this->nodes.begin(), SingleNode);
	this->nodes.insert(this->nodes.begin()+5, SingleNode);

	node_item = this->nodes.begin()+3;
	this->nodes.insert(node_item, SingleNode);
	//node_item += 1;
	//Nach Zuordnung eines Start-pointers, kann man diesen mit den Operatoren "++" "+=1" hochzaehlen und einen neuen Knoten an der resultierenden Stelle einfügen:
	node_item = this->nodes.begin() + 3;
	++node_item;
	node_item += 1;
	this->nodes.insert(node_item, SingleNode);
	
	//Nachdem diese Adresse von "node_item" verwendet wurde, kann sie nicht weiter "iteriert" werden:
	// > node_item += 1; => fuehrt zu Fehler
	// Alter "Wert" für "node_item" ist verloren gegangen

}
