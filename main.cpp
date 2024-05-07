#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>

class Player {
public:
    std::string id;
    char sign;
    int wins;
    int draws;
    int loses;
    int points;

    Player(const std::string& id, char sign) : id(id), sign(sign), wins(0), draws(0), loses(0), points(0) {}
};

void updateScores(std::unordered_map<char, Player>& players, char winner) {
    for (auto& pair : players) {
        if (pair.second.sign == winner)
            pair.second.wins++;
        else if (winner != 'n')
            pair.second.loses++;
        else
            pair.second.draws++;
        pair.second.points = pair.second.wins * 3 + pair.second.draws;
    }
}

char checkWinner(const std::vector<std::string>& board) {
    // Sprawdzenie poziomo
    for (int i = 0; i < 3; ++i) {
        if (board[i][0] == board[i][1] && board[i][1] == board[i][2] && board[i][0] != '.')
            return board[i][0];
    }

    // Sprawdzenie pionowo
    for (int i = 0; i < 3; ++i) {
        if (board[0][i] == board[1][i] && board[1][i] == board[2][i] && board[0][i] != '.')
            return board[0][i];
    }

    // Sprawdzenie na ukos
    if (board[0][0] == board[1][1] && board[1][1] == board[2][2] && board[0][0] != '.')
        return board[0][0];
    if (board[0][2] == board[1][1] && board[1][1] == board[2][0] && board[0][2] != '.')
        return board[0][2];

    // Jeśli nikt nie wygrał, zwróć 'n' jako informację o remisie
    return 'n';
}


int main() {
    std::ifstream file("tictactoe.txt");
    if (!file.is_open()) {
        std::cerr << "Nie można otworzyć pliku tictactoe.txt!" << std::endl;
        return 1;
    }

    // Wczytanie graczy z pliku players.txt
    std::ifstream playersFile("players.txt");
    if (!playersFile.is_open()) {
        std::cerr << "Nie można otworzyć pliku players.txt!" << std::endl;
        return 1;
    }

    std::unordered_map<char, Player> players;
    std::string playerId;
    char sign;
    while (playersFile >> playerId >> sign) {
        players.emplace(sign, Player(playerId, sign));
    }
    playersFile.close();

    std::vector<std::string> board(3);
    std::string line;
    int round = 1;

    while (std::getline(file, line)) {
        if (line.empty()) { // Pusta linia oznacza koniec rundy
            char winner = checkWinner(board);
            updateScores(players, winner);
            std::cout << "Runda " << round << ": ";
            if (winner != 'n')
                std::cout << "Wygrywa gracz " << winner << std::endl;
            else
                std::cout << "Remis" << std::endl;

            // Wyczyszczenie planszy
            board = std::vector<std::string>(3);
            round++;
        } else {
            for (int i = 0; i < 3; ++i)
                board[i] += line[i];
        }
    }

    file.close();

    // Wyświetlenie wyników w postaci tabeli
    std::cout << "ID\tSign\tWins\tDraws\tLoses\tPoints" << std::endl;
    for (const auto& pair : players) {
        const auto& player = pair.second;
        std::cout << player.id << "\t" << player.sign << "\t" << player.wins << "\t"
                  << player.draws << "\t" << player.loses << "\t" << player.points << std::endl;
    }

    return 0;
}
