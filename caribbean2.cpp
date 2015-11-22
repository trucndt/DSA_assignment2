/*
 * =========================================================================================
 * Name        : caribbean2.cpp
 * Author      : Nguyen Hoang Minh
 * Email       : nghoangminh1505@gmail.com
 * Copyright   : Faculty of Computer Science and Engineering - HCMC University of Technology
 * Description : Initial code for Assignment 2 - Data structures and Algorithms - Oct 2015
 * =========================================================================================
 */

//REMEMBER: Do not include any other library. Otherwise you will get 0 mark. :P

#ifndef _DEFS_H_
#include "defs.h"
#define _DEFS_H_
#endif

struct queue
{
	nodeTree *ptr;
	queue *next;
};

void adventure(eventList*, pirateTree*&);
int getHeight(nodeTree*);
void copyTree();
bool addNode(nodeTree*);
void delNode(int);
int getCount(nodeTree*);
int extractEvent(int&);
void addShip(int);
void monsterAttack(int);
void monsterAttack2(int);
void Bermuda();
void Poseiden();
void deleteMyTree(nodeTree*&);
void soulLand();
void deadManNoTell();
int _pow(int x, int y);

pirateTree *myTree;
bool isAVL;
bool isSoulLand = 0, isPosiden = 0, isBermuda = 0, isSteeled = 0, isJack = 0;

void adventure(eventList* pEvent, pirateTree*& aTree)
{
	int eventCode;

	myTree = aTree;

	while (pEvent != NULL)
	{
		eventCode = pEvent->nEventCode;
		switch (extractEvent(eventCode))
		{
		case 1:
			if (!isSoulLand) addShip(eventCode);
			break;
		case 2:
			if (!isPosiden) monsterAttack(eventCode);
			break;
		case 3:
			if (!isBermuda || isSteeled) Bermuda();
			break;
		case 4:
			if (!isSteeled) Poseiden();
			break;
		case 5:
			if (isSteeled) monsterAttack2(eventCode);
			break;
		case 6:
			if (isSoulLand) break;
			if (isSteeled) soulLand(); else deleteMyTree(myTree->root);
			break;
		case 7:
			if (isSoulLand && isJack) deadManNoTell();
			break;
		}
		if (myTree->root == NULL) break;
		pEvent = pEvent->pNext;
	}
}

int extractEvent(int &eventCode)
{
	eventCode = (eventCode > 0)? eventCode: (eventCode * -1);
	if (eventCode == 777) return 3;
	if (eventCode == 11111) return 4;
	if (eventCode == 5) return 6;
	if (eventCode == 6) return 7;
	if (eventCode == 7) return 8;

	if (eventCode / 10000 == 1) return 1;
	if (eventCode / 10000 == 2) return 2;
	if (eventCode / 10000 == 3) return 5;
	if (eventCode / 10 == 8) return 9;

	return 0;
}

void addShip(int code)
{
	int level = code % 10;
	if (level < 2 || level > 7) return;
	int key = code / 10 % 1000;

	nodeTree *ship = new nodeTree;
	ship->balance = 0;
	ship->exp = 0;
	ship->key = key;
	ship->level = level;
	ship->pLeft = NULL;
	ship->pRight = NULL;

	while (!addNode(ship))
	{
		ship->key--;
		if (ship->key < 0) break;
	}
	if (ship->key < 0) delete ship;
}

void monsterAttack(int code)
{
	int levelO = code % 10;
	int keyO = code / 10 % 1000;
	nodeTree *root = myTree->root;
	nodeTree *attackShip = myTree->root;

	while (root != NULL)
	{
		if (root->key < keyO)
		{
			attackShip = root;
			root = root->pRight;
		}
		else root = root->pLeft;
	}

	if (attackShip->level <= levelO) delNode(attackShip->key);
	else
	{
		int delta = 1000 / (((attackShip->level - levelO) + 1) * (attackShip->level + 1));
		if (attackShip->level == 7)
		{
			attackShip->exp = (100 < (attackShip->exp + delta))? 100 : (attackShip->exp + delta);
		}
		else
		{
			attackShip->exp += delta;
			while (attackShip->exp >= 100)
			{
				attackShip->level += 1;
				attackShip->exp -= 100;
			}
		}
		//cout << attackShip->exp << "\n";
	}

}

nodeTree* deleteLeafRNL(nodeTree *root)
{
	if (root == NULL) return NULL;

	if (root->pLeft == NULL && root->pRight == NULL)
	{
		delete root;
		return NULL;
	}

	root->pRight = deleteLeafRNL(root->pRight);
	root->pLeft = deleteLeafRNL(root->pLeft);

	return root;
}

void Bermuda()
{
	myTree->root = deleteLeafRNL(myTree->root);
}

void deleteLevel(nodeTree *root, int level, int rLevel)
{
	if (root == NULL) return;
	if (level == rLevel)
	{
		delNode(root->key);
	}
	else
	{
		deleteLevel(root->pLeft, level + 1, rLevel);
		deleteLevel(root->pRight, level + 1, rLevel);
	}
}

bool sidaShip(nodeTree *root)
{
	if (root == NULL) return 0;
	if (root->key % 111 == 0) return 1;
	bool sidaLeft = sidaShip(root->pLeft);
	bool sidaRight = sidaShip(root->pRight);
	return sidaLeft | sidaRight;
}

void Poseiden()
{
	int height = getHeight(myTree->root);
	if (sidaShip(myTree->root))
	{
		int n = (height - 2) > 0? (height - 2):0;
		deleteLevel(myTree->root, 0, n);
	}
	else isSteeled = 1;
}

void findNearestKey(nodeTree* root, nodeTree*& retNode, int retKey, int key)
{
	if (root == NULL) return;
	int absKey = root->key - key;
	if (absKey < 0) absKey *= -1;

	if (absKey < retKey || (absKey == retKey && retNode->key > root->key))
	{
		retKey = absKey;
		retNode = root;
	}

	findNearestKey(root->pLeft, retNode, retKey, key);
	findNearestKey(root->pRight, retNode, retKey, key);
}

void monsterAttack2(int code)
{
	int levelO = code % 10;
	int keyO = code / 10 % 1000;
	nodeTree *attackShip = myTree->root;
	int cnt = 3;
	while (cnt)
	{
		findNearestKey(myTree->root, attackShip, 1000, keyO);

		int absKey = keyO - attackShip->key;
		if (absKey < 0) absKey *= -1;

		if (attackShip->level <= levelO)
		{
			keyO += 3*absKey;
			while (keyO > 999) keyO -= 1000;

			delNode(attackShip->key);
		}
		else
		{
			int delta = 1000 / (((attackShip->level - levelO) + 1) * (attackShip->level + 1));
			if (attackShip->level == 7)
			{
				attackShip->exp = (100 < (attackShip->exp + delta))? 100 : (attackShip->exp + delta);
			}
			else
			{
				attackShip->exp += delta;
				while (attackShip->exp >= 100)
				{
					attackShip->level += 1;
					attackShip->exp -= 100;
				}

			}
			cnt--;
			keyO -= 3*absKey;
			while (keyO < 0) keyO += 1000;
			//cout << attackShip->exp << "\n";
		}
		if (myTree->root == NULL) return;
	}
}

void insertNLR(nodeTree* root)
{
	if (root == NULL) return;
	nodeTree *node = new nodeTree;
	node->balance = root->balance;
	node->exp = root->exp;
	node->key = root->key;
	node->level = root->level;
	node->pLeft = NULL;
	node->pRight = NULL;

	addNode(node);
	insertNLR(root->pLeft);
	insertNLR(root->pRight);

	delete root;
}

void incLevel(nodeTree *root)
{
	if (root == NULL) return;
	if (root->level < 7) root->level++;
	incLevel(root->pLeft);
	incLevel(root->pRight);
}

void soulLand()
{
	nodeTree *oldRoot = myTree->root;
	myTree->root = new nodeTree();
	myTree->root->balance = 0;
	myTree->root->exp = 0;
	myTree->root->key = 777;
	myTree->root->level = 7;
	myTree->root->pLeft = NULL;
	myTree->root->pRight = NULL;

	insertNLR(oldRoot);
	isSoulLand = 1;
	isJack = 1;
	if (_pow(2, getHeight(myTree->root)) - 1 == getCount(myTree->root))
		incLevel(myTree->root);
}

void listLNR(nodeTree *root, queue *&rear)
{
	if (root == NULL) return;
	listLNR(root->pLeft, rear);
	rear->next = new queue;
	rear = rear->next;
	rear->next = NULL;
	rear->ptr = root;
	listLNR(root->pRight, rear);
}

void deadManNoTell()
{
	//Find choosen Node
	nodeTree *choosenNode = NULL;
	queue *front = new queue;
	queue *rear = front;
	rear->next = NULL;

	int cnt = 1;
	int j = getHeight(myTree->root);
	j = (j > 4)? j/2 : 1;
	rear->ptr = myTree->root;

	while (front != NULL)
	{
		nodeTree *node = front->ptr;
		queue *del = front;
		front = front->next;
		delete del;

		if (node->pLeft != NULL)
		{
			cnt++;
			if (cnt == j)
			{
				choosenNode = node->pLeft;
				break;
			}
			rear->next = new queue;
			rear = rear->next;
			rear->ptr = node->pLeft;
			rear->next = NULL;
		}
		if (node->pRight != NULL)
		{
			cnt++;
			if (cnt == j)
			{
				choosenNode = node->pRight;
				break;
			}
			rear->next = new queue;
			rear = rear->next;
			rear->ptr = node->pRight;
			rear->next = NULL;
		}
	}

	while (front != NULL)
	{
		queue *del = front;
		front = front->next;
		delete del;
	}

	//Find branch
	bool flag = 0;
	for (int i = 0; i <= 1; i++)
	{
		front = new queue;
		front->next = NULL;
		rear = front;

		int way = i;
		cnt = 0;
		nodeTree *node = choosenNode;
		while (node != NULL)
		{
			rear->next = new queue;
			rear = rear->next;
			rear->next = NULL;
			rear->ptr = node;
			cnt++;
			if (cnt == 4) flag = 1;

			if (way == 0) node = node->pLeft;
			else node = node->pRight;
			way = !(way);
		}
		if (flag) break;
		while (front != NULL)
		{
			queue *del = front;
			front = front->next;
			delete del;
		}
	}

	if (!flag)
	{
		front = new queue;
		front->next = NULL;
		rear = front;
		listLNR(myTree->root->pLeft, rear);
	}
	else
	{
		if (j == 1) isJack = 0;
	}

	//delete branch
	while (front->next != NULL)
	{
		queue *del = front;
		delNode(front->next->ptr->key);
		front = front->next;
		delete del;
	}
	delete front;
}

/*----------------------------------------------------------*/
bool BSTinsert(nodeTree *&root, nodeTree* node)
{
	if (root == NULL)
	{
		root = node;
		return 1;
	}
	if (node->key < root->key) return BSTinsert(root->pLeft, node);
	if (node->key > root->key) return BSTinsert(root->pRight, node);
	return 0;
}

bool addNode(nodeTree *node)
{
	if (!isAVL)
	{
		return BSTinsert(myTree->root, node);
	}
	else
	{
		return 1;
	}
}

nodeTree* BSTremove(nodeTree *root, int key)
{
	if (root == NULL) return root;
	if (key < root->key) root->pLeft = BSTremove(root->pLeft, key);
	else if (key > root->key) root->pRight = BSTremove(root->pRight, key);
	else
	{
        if (root->pLeft == NULL)
        {
            nodeTree *temp = root->pRight;
            delete root;
            return temp;
        }
        else if (root->pRight == NULL)
        {
            nodeTree *temp = root->pLeft;
            delete root;
            return temp;
        }

        nodeTree *temp = root->pRight;
        while (temp->pLeft != NULL) temp = temp->pLeft;

        root->key = temp->key;
        root->exp = temp->exp;
        root->balance = temp->balance;
        root->level = temp->level;

        root->pRight = BSTremove(root->pRight, temp->key);
	}

	return root;
}

void delNode(int key)
{
	if (!isAVL)
	{
		myTree->root = BSTremove(myTree->root, key);
	}
	else
	{

	}
}

int getHeight(nodeTree* root)
{
	if (root == NULL) return 0;
	int leftHeight = getHeight(root->pLeft);
	int rightHeight = getHeight(root->pRight);
	return (leftHeight > rightHeight)? leftHeight + 1 : rightHeight + 1;
}

void deleteMyTree (nodeTree *&root)
{
	if (!root) return;
	nodeTree *pDel = root;
	deleteMyTree(root->pLeft);
	deleteMyTree(root->pRight);
	delete pDel; root = NULL;
}

int getCount(nodeTree* root)
{
	if (root == NULL) return 0;
	return 1 + getCount(root->pLeft) + getCount(root->pRight);
}

int _pow(int x, int y)
{
	if (y == 0) return 1;
	return x * pow(x,y - 1);
}