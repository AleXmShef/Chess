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
	//first pass to set areCutting flag
	for (int k = 0; k < 8; k++) {
		for (int l = 0; l < 8; l++) {
			if ((*(*mCellBoard)[k])[l]->chip != nullptr) {
				auto moveVec = new std::vector<Move^>;
				auto tchip = (*(*mCellBoard)[k])[l]->chip;
				if (tchip->colour == mGameSide) {
					switch (tchip->type) {
					case ChipType::Regular:
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
							areCutting = true;
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
							areCutting = true;
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
							areCutting = true;
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
							areCutting = true;
							tmove->cuttedChip = (*(*mCellBoard)[k - 1])[l - 1]->chip;
							moveVec->push_back(tmove);
						}
					}
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

void Board::move(Move^ move) {
	auto tchip = (*(*mCellBoard)[move->fromXY.second])[move->fromXY.first]->chip;
	auto jsonMove = ref new JsonObject();
	jsonMove->Insert("fromX", JsonValue::CreateNumberValue(move->fromXY.first));
	jsonMove->Insert("fromY", JsonValue::CreateNumberValue(move->fromXY.second));
	jsonMove->Insert("toX", JsonValue::CreateNumberValue(move->toXY.first));
	jsonMove->Insert("toY", JsonValue::CreateNumberValue(move->toXY.second));
	jsonMove->Insert("isCutting", JsonValue::CreateBooleanValue(move->isCutting));
	int x;
	int y;
	if (move->isCutting) {
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				if ((*(*mCellBoard)[i])[j]->chip = move->cuttedChip) {
					x = j;
					y = i;
				}
			}
		}
		jsonMove->Insert("cuttedChipX", JsonValue::CreateNumberValue(x));
		jsonMove->Insert("cuttedChipY", JsonValue::CreateNumberValue(y));
	}
	(*(*mCellBoard)[move->toXY.second])[move->toXY.first]->chip = tchip;
	(*(*mCellBoard)[move->fromXY.second])[move->fromXY.first]->chip = nullptr;
	if (move->isCutting) {
		(*(*mCellBoard)[y])[x]->chip = nullptr;
	}
	Game::getInstance()->sendMove(jsonMove);
}

void Board::moveFromJson(JsonObject^ jsonMove) {
	int fromX = jsonMove->GetNamedNumber("fromX");
	int fromY = jsonMove->GetNamedNumber("fromY");
	int toX = jsonMove->GetNamedNumber("toX");
	int toY = jsonMove->GetNamedNumber("toY");
	bool isCutting = jsonMove->GetNamedBoolean("isCutting");
	int cX, cY;
	if (isCutting) {
		cX = jsonMove->GetNamedNumber("cuttedX");
		cY = jsonMove->GetNamedNumber("cuttedY");
	}
	auto tchip = (*(*mCellBoard)[fromY])[fromX]->chip;
	(*(*mCellBoard)[toY])[toX]->chip = tchip;
	(*(*mCellBoard)[fromY])[fromX]->chip = nullptr;
	if (isCutting) {
		(*(*mCellBoard)[cY])[cX]->chip = nullptr;
	}
}