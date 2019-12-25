#include "pch.h"
#include "Board.h"
#include "Game.h"

using namespace App1;

using namespace Windows::Data::Json;

Board^ Board::mInstance = nullptr;

Board^ Board::getInstance() {
	if (mInstance == nullptr) {
		mInstance = ref new Board();
	}
	return mInstance;
}

Board::Board() {

};

void Board::moveAvailibilityPass() {
	//mMoveAvailibilityPassMap->clear();
	mMoveAvailibilityPassMap = new std::map<std::pair<int, int>, std::vector<Move^>*>;
	bool areCutting = false;
	for (int k = 0; k < 8; k++) {
		for (int l = 0; l < 8; l++) {
			if ((*(*mCellBoard)[k])[l]->chip != nullptr) {
				auto moveVec = new std::vector<Move^>;
				auto tchip = (*(*mCellBoard)[k])[l]->chip;
				if (tchip->colour == mGameSide) {
					moveVec = findMovesForCurrentChip(std::pair<int, int>(l, k), std::pair<int, int>(-1, -1), tchip->type);
					for (int i = 0; i < moveVec->size(); i++) {
						if ((*moveVec)[i]->isCutting)
							areCutting = true;
					}
				}
				if (!moveVec->empty()) {
					std::pair<int,int> pair;
					pair.first = l;
					pair.second = k;
					mMoveAvailibilityPassMap->insert({ pair, moveVec });
				}
			}
		}
	}
	if (areCutting) {
		for (std::map<std::pair<int, int>, std::vector<Move^>*>::iterator it = mMoveAvailibilityPassMap->begin(); it != mMoveAvailibilityPassMap->end(); it++) {
			for (int i = 0; i < it->second->size(); i++) {
				if (!(*(it->second))[i]->isCutting) {
					(it->second)->erase(it->second->begin() + i);
					if (it->second->empty()) {
						mMoveAvailibilityPassMap->erase(it);
					}
				}
			}
		}
	}
}

std::vector<std::vector<Cell^>*>* Board::getCellBoard() {
	return mCellBoard;
};

void Board::populateBoard(GameSide side) {
	mGameSide = side;
	mCellBoard = new std::vector<std::vector<Cell^>*>;
	for (int i = 0; i < 8; i++) {
		auto tvec = new std::vector<Cell^>;
		for (int j = 0; j < 8; j++) {
			auto tcell = ref new Cell();
			tvec->push_back(tcell);
		}
		mCellBoard->push_back(tvec);
	}
	if (side == GameSide::White) {
		(*(*mCellBoard)[0])[0]->chip = ref new Chip();
		(*(*mCellBoard)[0])[0]->chip->type = ChipType::Regular;
		(*(*mCellBoard)[0])[0]->chip->colour = GameSide::White;
		(*(*mCellBoard)[0])[2]->chip = ref new Chip();
		(*(*mCellBoard)[0])[2]->chip->type = ChipType::Regular;
		(*(*mCellBoard)[0])[2]->chip->colour = GameSide::White;
		(*(*mCellBoard)[0])[4]->chip = ref new Chip();
		(*(*mCellBoard)[0])[4]->chip->type = ChipType::Regular;
		(*(*mCellBoard)[0])[4]->chip->colour = GameSide::White;
		(*(*mCellBoard)[0])[6]->chip = ref new Chip();
		(*(*mCellBoard)[0])[6]->chip->type = ChipType::Regular;
		(*(*mCellBoard)[0])[6]->chip->colour = GameSide::White;
		(*(*mCellBoard)[1])[1]->chip = ref new Chip();
		(*(*mCellBoard)[1])[1]->chip->type = ChipType::Regular;
		(*(*mCellBoard)[1])[1]->chip->colour = GameSide::White;
		(*(*mCellBoard)[1])[3]->chip = ref new Chip();
		(*(*mCellBoard)[1])[3]->chip->type = ChipType::Regular;
		(*(*mCellBoard)[1])[3]->chip->colour = GameSide::White;
		(*(*mCellBoard)[1])[5]->chip = ref new Chip();
		(*(*mCellBoard)[1])[5]->chip->type = ChipType::Regular;
		(*(*mCellBoard)[1])[5]->chip->colour = GameSide::White;
		(*(*mCellBoard)[1])[7]->chip = ref new Chip();
		(*(*mCellBoard)[1])[7]->chip->type = ChipType::Regular;
		(*(*mCellBoard)[1])[7]->chip->colour = GameSide::White;
		(*(*mCellBoard)[2])[0]->chip = ref new Chip();
		(*(*mCellBoard)[2])[0]->chip->type = ChipType::Regular;
		(*(*mCellBoard)[2])[0]->chip->colour = GameSide::White;
		(*(*mCellBoard)[2])[2]->chip = ref new Chip();
		(*(*mCellBoard)[2])[2]->chip->type = ChipType::Regular;
		(*(*mCellBoard)[2])[2]->chip->colour = GameSide::White;
		(*(*mCellBoard)[2])[4]->chip = ref new Chip();
		(*(*mCellBoard)[2])[4]->chip->type = ChipType::Regular;
		(*(*mCellBoard)[2])[4]->chip->colour = GameSide::White;
		(*(*mCellBoard)[2])[6]->chip = ref new Chip();
		(*(*mCellBoard)[2])[6]->chip->type = ChipType::Regular;
		(*(*mCellBoard)[2])[6]->chip->colour = GameSide::White;

		(*(*mCellBoard)[5])[1]->chip = ref new Chip();
		(*(*mCellBoard)[5])[1]->chip->type = ChipType::Regular;
		(*(*mCellBoard)[5])[1]->chip->colour = GameSide::Brown;
		(*(*mCellBoard)[5])[3]->chip = ref new Chip();
		(*(*mCellBoard)[5])[3]->chip->type = ChipType::Regular;
		(*(*mCellBoard)[5])[3]->chip->colour = GameSide::Brown;
		(*(*mCellBoard)[5])[5]->chip = ref new Chip();
		(*(*mCellBoard)[5])[5]->chip->type = ChipType::Regular;
		(*(*mCellBoard)[5])[5]->chip->colour = GameSide::Brown;
		(*(*mCellBoard)[5])[7]->chip = ref new Chip();
		(*(*mCellBoard)[5])[7]->chip->type = ChipType::Regular;
		(*(*mCellBoard)[5])[7]->chip->colour = GameSide::Brown;
		(*(*mCellBoard)[6])[0]->chip = ref new Chip();
		(*(*mCellBoard)[6])[0]->chip->type = ChipType::Regular;
		(*(*mCellBoard)[6])[0]->chip->colour = GameSide::Brown;
		(*(*mCellBoard)[6])[2]->chip = ref new Chip();
		(*(*mCellBoard)[6])[2]->chip->type = ChipType::Regular;
		(*(*mCellBoard)[6])[2]->chip->colour = GameSide::Brown;
		(*(*mCellBoard)[6])[4]->chip = ref new Chip();
		(*(*mCellBoard)[6])[4]->chip->type = ChipType::Regular;
		(*(*mCellBoard)[6])[4]->chip->colour = GameSide::Brown;
		(*(*mCellBoard)[6])[6]->chip = ref new Chip();
		(*(*mCellBoard)[6])[6]->chip->type = ChipType::Regular;
		(*(*mCellBoard)[6])[6]->chip->colour = GameSide::Brown;
		(*(*mCellBoard)[7])[1]->chip = ref new Chip();
		(*(*mCellBoard)[7])[1]->chip->type = ChipType::Regular;
		(*(*mCellBoard)[7])[1]->chip->colour = GameSide::Brown;
		(*(*mCellBoard)[7])[3]->chip = ref new Chip();
		(*(*mCellBoard)[7])[3]->chip->type = ChipType::Regular;
		(*(*mCellBoard)[7])[3]->chip->colour = GameSide::Brown;
		(*(*mCellBoard)[7])[5]->chip = ref new Chip();
		(*(*mCellBoard)[7])[5]->chip->type = ChipType::Regular;
		(*(*mCellBoard)[7])[5]->chip->colour = GameSide::Brown;
		(*(*mCellBoard)[7])[7]->chip = ref new Chip();
		(*(*mCellBoard)[7])[7]->chip->type = ChipType::Regular;
		(*(*mCellBoard)[7])[7]->chip->colour = GameSide::Brown;
	}
	else {
		(*(*mCellBoard)[0])[0]->chip = ref new Chip();
		(*(*mCellBoard)[0])[0]->chip->type = ChipType::Regular;
		(*(*mCellBoard)[0])[0]->chip->colour = GameSide::Brown;
		(*(*mCellBoard)[0])[2]->chip = ref new Chip();
		(*(*mCellBoard)[0])[2]->chip->type = ChipType::Regular;
		(*(*mCellBoard)[0])[2]->chip->colour = GameSide::Brown;
		(*(*mCellBoard)[0])[4]->chip = ref new Chip();
		(*(*mCellBoard)[0])[4]->chip->type = ChipType::Regular;
		(*(*mCellBoard)[0])[4]->chip->colour = GameSide::Brown;
		(*(*mCellBoard)[0])[6]->chip = ref new Chip();
		(*(*mCellBoard)[0])[6]->chip->type = ChipType::Regular;
		(*(*mCellBoard)[0])[6]->chip->colour = GameSide::Brown;
		(*(*mCellBoard)[1])[1]->chip = ref new Chip();
		(*(*mCellBoard)[1])[1]->chip->type = ChipType::Regular;
		(*(*mCellBoard)[1])[1]->chip->colour = GameSide::Brown;
		(*(*mCellBoard)[1])[3]->chip = ref new Chip();
		(*(*mCellBoard)[1])[3]->chip->type = ChipType::Regular;
		(*(*mCellBoard)[1])[3]->chip->colour = GameSide::Brown;
		(*(*mCellBoard)[1])[5]->chip = ref new Chip();
		(*(*mCellBoard)[1])[5]->chip->type = ChipType::Regular;
		(*(*mCellBoard)[1])[5]->chip->colour = GameSide::Brown;
		(*(*mCellBoard)[1])[7]->chip = ref new Chip();
		(*(*mCellBoard)[1])[7]->chip->type = ChipType::Regular;
		(*(*mCellBoard)[1])[7]->chip->colour = GameSide::Brown;
		(*(*mCellBoard)[2])[0]->chip = ref new Chip();
		(*(*mCellBoard)[2])[0]->chip->type = ChipType::Regular;
		(*(*mCellBoard)[2])[0]->chip->colour = GameSide::Brown;
		(*(*mCellBoard)[2])[2]->chip = ref new Chip();
		(*(*mCellBoard)[2])[2]->chip->type = ChipType::Regular;
		(*(*mCellBoard)[2])[2]->chip->colour = GameSide::Brown;
		(*(*mCellBoard)[2])[4]->chip = ref new Chip();
		(*(*mCellBoard)[2])[4]->chip->type = ChipType::Regular;
		(*(*mCellBoard)[2])[4]->chip->colour = GameSide::Brown;
		(*(*mCellBoard)[2])[6]->chip = ref new Chip();
		(*(*mCellBoard)[2])[6]->chip->type = ChipType::Regular;
		(*(*mCellBoard)[2])[6]->chip->colour = GameSide::Brown;

		(*(*mCellBoard)[5])[1]->chip = ref new Chip();
		(*(*mCellBoard)[5])[1]->chip->type = ChipType::Regular;
		(*(*mCellBoard)[5])[1]->chip->colour = GameSide::White;
		(*(*mCellBoard)[5])[3]->chip = ref new Chip();
		(*(*mCellBoard)[5])[3]->chip->type = ChipType::Regular;
		(*(*mCellBoard)[5])[3]->chip->colour = GameSide::White;
		(*(*mCellBoard)[5])[5]->chip = ref new Chip();
		(*(*mCellBoard)[5])[5]->chip->type = ChipType::Regular;
		(*(*mCellBoard)[5])[5]->chip->colour = GameSide::White;
		(*(*mCellBoard)[5])[7]->chip = ref new Chip();
		(*(*mCellBoard)[5])[7]->chip->type = ChipType::Regular;
		(*(*mCellBoard)[5])[7]->chip->colour = GameSide::White;
		(*(*mCellBoard)[6])[0]->chip = ref new Chip();
		(*(*mCellBoard)[6])[0]->chip->type = ChipType::Regular;
		(*(*mCellBoard)[6])[0]->chip->colour = GameSide::White;
		(*(*mCellBoard)[6])[2]->chip = ref new Chip();
		(*(*mCellBoard)[6])[2]->chip->type = ChipType::Regular;
		(*(*mCellBoard)[6])[2]->chip->colour = GameSide::White;
		(*(*mCellBoard)[6])[4]->chip = ref new Chip();
		(*(*mCellBoard)[6])[4]->chip->type = ChipType::Regular;
		(*(*mCellBoard)[6])[4]->chip->colour = GameSide::White;
		(*(*mCellBoard)[6])[6]->chip = ref new Chip();
		(*(*mCellBoard)[6])[6]->chip->type = ChipType::Regular;
		(*(*mCellBoard)[6])[6]->chip->colour = GameSide::White;
		(*(*mCellBoard)[7])[1]->chip = ref new Chip();
		(*(*mCellBoard)[7])[1]->chip->type = ChipType::Regular;
		(*(*mCellBoard)[7])[1]->chip->colour = GameSide::White;
		(*(*mCellBoard)[7])[3]->chip = ref new Chip();
		(*(*mCellBoard)[7])[3]->chip->type = ChipType::Regular;
		(*(*mCellBoard)[7])[3]->chip->colour = GameSide::White;
		(*(*mCellBoard)[7])[5]->chip = ref new Chip();
		(*(*mCellBoard)[7])[5]->chip->type = ChipType::Regular;
		(*(*mCellBoard)[7])[5]->chip->colour = GameSide::White;
		(*(*mCellBoard)[7])[7]->chip = ref new Chip();
		(*(*mCellBoard)[7])[7]->chip->type = ChipType::Regular;
		(*(*mCellBoard)[7])[7]->chip->colour = GameSide::White;
	}
}

std::map<std::pair<int, int>, std::vector<Move^>*>* Board::getMoveAvailibilityPassMap() {
	return mMoveAvailibilityPassMap;
}

void Board::move(std::vector<Move^>* moves) {
	auto jsonMoves = ref new JsonObject();
	auto jsonMovesArray = ref new JsonArray();
	for (int i = 0; i < moves->size(); i++) {
		auto move = (*moves)[i];
		auto tchip = (*(*mCellBoard)[move->fromXY.second])[move->fromXY.first]->chip;
		auto jsonMove = ref new JsonObject();
		jsonMove->Insert("fromX", JsonValue::CreateNumberValue(move->fromXY.first));
		jsonMove->Insert("fromY", JsonValue::CreateNumberValue(move->fromXY.second));
		jsonMove->Insert("toX", JsonValue::CreateNumberValue(move->toXY.first));
		jsonMove->Insert("toY", JsonValue::CreateNumberValue(move->toXY.second));
		jsonMove->Insert("isCutting", JsonValue::CreateBooleanValue(move->isCutting));
		jsonMovesArray->Append(jsonMove);
		int x;
		int y;
		if (move->isCutting) {
			for (int i = 0; i < 8; i++) {
				for (int j = 0; j < 8; j++) {
					if ((*(*mCellBoard)[i])[j]->chip == move->cuttedChip) {
						x = j;
						y = i;
					}
				}
			}
			jsonMove->Insert("cuttedX", JsonValue::CreateNumberValue(x));
			jsonMove->Insert("cuttedY", JsonValue::CreateNumberValue(y));
		}
		(*(*mCellBoard)[move->toXY.second])[move->toXY.first]->chip = tchip;
		(*(*mCellBoard)[move->fromXY.second])[move->fromXY.first]->chip = nullptr;
		if (move->isCutting) {
			(*(*mCellBoard)[y])[x]->chip = nullptr;
		}
		if (move->toXY.second == 7) {
			(*(*mCellBoard)[move->toXY.second])[move->toXY.first]->chip->type = ChipType::Queen;
		}
	}
	int opponentChips = 0;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if ((*(*mCellBoard)[i])[j]->chip != nullptr)
				if ((*(*mCellBoard)[i])[j]->chip->colour != mGameSide)
					opponentChips++;
		}
	}
	jsonMoves->Insert("Moves", jsonMovesArray);
	Game::getInstance()->sendMove(jsonMoves);
	if (opponentChips == 0) {
		Game::getInstance()->win();
		OutputDebugString(L"Game won");
	}
}

void Board::moveFromJson(JsonObject^ jsonMoves) {
	auto jsonMovesArray = jsonMoves->GetNamedArray("Moves");
	for (unsigned int i = 0; i < jsonMovesArray->Size; i++) {
		auto jsonMove = jsonMovesArray->GetAt(i)->GetObject();
		int fromX = 7 - jsonMove->GetNamedNumber("fromX");
		int fromY = 7 - jsonMove->GetNamedNumber("fromY");
		int toX = 7 - jsonMove->GetNamedNumber("toX");
		int toY = 7 - jsonMove->GetNamedNumber("toY");
		bool isCutting = jsonMove->GetNamedBoolean("isCutting");
		int cX, cY;
		if (isCutting) {
			cX = 7 - jsonMove->GetNamedNumber("cuttedX");
			cY = 7 - jsonMove->GetNamedNumber("cuttedY");
		}
		auto tchip = (*(*mCellBoard)[fromY])[fromX]->chip;
		(*(*mCellBoard)[toY])[toX]->chip = tchip;
		(*(*mCellBoard)[fromY])[fromX]->chip = nullptr;
		if (isCutting) {
			(*(*mCellBoard)[cY])[cX]->chip = nullptr;
		}
		if (toY == 0) {
			(*(*mCellBoard)[toY])[toX]->chip->type = ChipType::Queen;
		}
	}
	int myChips = 0;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if ((*(*mCellBoard)[i])[j]->chip != nullptr)
				if ((*(*mCellBoard)[i])[j]->chip->colour == mGameSide)
					myChips++;
		}
	}
	if (myChips == 0) {
		OutputDebugString(L"Game lost\n");
		Game::getInstance()->lose();
	}
}

std::vector<Move^>* Board::findMovesForCurrentChip(std::pair<int, int> chip,std::pair<int,int> exclude, ChipType type) {
	int l = chip.first;
	int k = chip.second;
	auto moveVec = new std::vector<Move^>;
	switch (type) {
	case ChipType::Regular:
	{
		if (exclude.first == -1 && exclude.second == -1) {
			if ((k + 1) < 8 && (l + 1) < 8 && ((*(*mCellBoard)[k + 1])[l + 1]->chip == nullptr)) {
				auto tmove = ref new Move();
				tmove->fromXY.first = l;
				tmove->fromXY.second = k;
				tmove->toXY.first = l + 1;
				tmove->toXY.second = k + 1;
				moveVec->push_back(tmove);
			}
			if ((k + 1) < 8 && (l - 1) >= 0 && ((*(*mCellBoard)[k + 1])[l - 1]->chip == nullptr)) {
				auto tmove = ref new Move();
				tmove->fromXY.first = l;
				tmove->fromXY.second = k;
				tmove->toXY.first = l - 1;
				tmove->toXY.second = k + 1;
				moveVec->push_back(tmove);
			}
		}
		if ((k + 2) < 8 && (l + 2) < 8 && ((*(*mCellBoard)[k + 1])[l + 1]->chip != nullptr) && (*(*mCellBoard)[k + 2])[l + 2]->chip==nullptr && (*(*mCellBoard)[k + 1])[l + 1]->chip->colour != mGameSide && k + 1 != exclude.second && l + 1 != exclude.first) {
			auto tmove = ref new Move();
			tmove->fromXY.first = l;
			tmove->fromXY.second = k;
			tmove->toXY.first = l + 2;
			tmove->toXY.second = k + 2;
			tmove->isCutting = true;
			tmove->cuttedChip = (*(*mCellBoard)[k + 1])[l + 1]->chip;
			moveVec->push_back(tmove);
		}
		if ((k + 2) < 8 && (l - 2) >= 0 && (*(*mCellBoard)[k + 1])[l - 1]->chip != nullptr && (*(*mCellBoard)[k + 2])[l - 2]->chip == nullptr && (*(*mCellBoard)[k + 1])[l - 1]->chip->colour != mGameSide && k + 1 != exclude.second && l - 1 != exclude.first) {
			auto tmove = ref new Move();
			tmove->fromXY.first = l;
			tmove->fromXY.second = k;
			tmove->toXY.first = l - 2;
			tmove->toXY.second = k + 2;
			tmove->isCutting = true;
			tmove->cuttedChip = (*(*mCellBoard)[k + 1])[l - 1]->chip;
			moveVec->push_back(tmove);
		}
		if (k - 2 >= 0 && l + 2 < 8 && (*(*mCellBoard)[k - 1])[l + 1]->chip != nullptr && (*(*mCellBoard)[k - 2])[l + 2]->chip && (*(*mCellBoard)[k - 1])[l + 1]->chip->colour != mGameSide && k - 1 != exclude.second && l + 1 != exclude.first) {
			auto tmove = ref new Move();
			tmove->fromXY.first = l;
			tmove->fromXY.second = k;
			tmove->toXY.first = l + 2;
			tmove->toXY.second = k - 2;
			tmove->isCutting = true;
			tmove->cuttedChip = (*(*mCellBoard)[k - 1])[l + 1]->chip;
			moveVec->push_back(tmove);
		}
		if ((k - 2) >= 0 && (l - 2) >= 0 && (*(*mCellBoard)[k - 1])[l - 1]->chip != nullptr && (*(*mCellBoard)[k - 2])[l - 2]->chip && (*(*mCellBoard)[k - 1])[l - 1]->chip->colour != mGameSide && k - 1 != exclude.second && l - 1 != exclude.first) {
			auto tmove = ref new Move();
			tmove->fromXY.first = l;
			tmove->fromXY.second = k;
			tmove->toXY.first = l - 2;
			tmove->toXY.second = k - 2;
			tmove->isCutting = true;
			tmove->cuttedChip = (*(*mCellBoard)[k - 1])[l - 1]->chip;
			moveVec->push_back(tmove);
		}
		break;
	}
	case ChipType::Queen:
	{
		if ((k + 1) < 8 && (l + 1) < 8 && ((*(*mCellBoard)[k + 1])[l + 1]->chip == nullptr)) {
			auto tmove = ref new Move();
			tmove->fromXY.first = l;
			tmove->fromXY.second = k;
			tmove->toXY.first = l + 1;
			tmove->toXY.second = k + 1;
			moveVec->push_back(tmove);
		}
		if ((k + 1) < 8 && (l - 1) >= 0 && ((*(*mCellBoard)[k + 1])[l - 1]->chip == nullptr)) {
			auto tmove = ref new Move();
			tmove->fromXY.first = l;
			tmove->fromXY.second = k;
			tmove->toXY.first = l - 1;
			tmove->toXY.second = k + 1;
			moveVec->push_back(tmove);
		}
		if ((k + 2) < 8 && (l + 2) < 8 && ((*(*mCellBoard)[k + 1])[l + 1]->chip != nullptr) && (*(*mCellBoard)[k + 1])[l + 1]->chip->colour != mGameSide) {
			auto tmove = ref new Move();
			tmove->fromXY.first = l;
			tmove->fromXY.second = k;
			tmove->toXY.first = l + 2;
			tmove->toXY.second = k + 2;
			tmove->isCutting = true;
			tmove->cuttedChip = (*(*mCellBoard)[k + 1])[l + 1]->chip;
			moveVec->push_back(tmove);
		}
		if ((k + 2) < 8 && (l - 2) >= 0 && (*(*mCellBoard)[k + 1])[l - 1]->chip != nullptr && (*(*mCellBoard)[k + 1])[l - 1]->chip->colour != mGameSide) {
			auto tmove = ref new Move();
			tmove->fromXY.first = l;
			tmove->fromXY.second = k;
			tmove->toXY.first = l - 2;
			tmove->toXY.second = k + 2;
			tmove->isCutting = true;
			tmove->cuttedChip = (*(*mCellBoard)[k + 1])[l - 1]->chip;
			moveVec->push_back(tmove);
		}
		if (k - 2 >= 0 && l + 2 < 8 && (*(*mCellBoard)[k - 1])[l + 1]->chip != nullptr && (*(*mCellBoard)[k - 1])[l + 1]->chip->colour != mGameSide) {
			auto tmove = ref new Move();
			tmove->fromXY.first = l;
			tmove->fromXY.second = k;
			tmove->toXY.first = l + 2;
			tmove->toXY.second = k - 2;
			tmove->isCutting = true;
			tmove->cuttedChip = (*(*mCellBoard)[k - 1])[l + 1]->chip;
			moveVec->push_back(tmove);
		}
		if ((k - 2) >= 0 && (l - 2) >= 0 && (*(*mCellBoard)[k - 1])[l - 1]->chip != nullptr && (*(*mCellBoard)[k - 1])[l - 1]->chip->colour != mGameSide) {
			auto tmove = ref new Move();
			tmove->fromXY.first = l;
			tmove->fromXY.second = k;
			tmove->toXY.first = l - 2;
			tmove->toXY.second = k - 2;
			tmove->isCutting = true;
			tmove->cuttedChip = (*(*mCellBoard)[k - 1])[l - 1]->chip;
			moveVec->push_back(tmove);
		}
		break;
	}
	}
	return moveVec;
}