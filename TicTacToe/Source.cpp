#include <SFML/Graphics.hpp>
#include<iostream>
using namespace std;

enum class Player {
    None, X, O
};

Player Board[3][3];

class Move {
public:
    int row;
    int col;

    Move() : row(-1), col(-1) {}
};

// initializes board with Player::None
void initBoard() {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            Board[i][j] = Player::None;
        }
    }
}

// Checks if board is full returns true if it is
bool isFull() {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (Board[i][j] == Player::None) {
                return false;
            }
        }
    }

    return true;
}

bool hasWon(Player player) {

    // Checking linear win conditions
    for (int i = 0; i < 3; ++i) {
        if (Board[i][0] == player && Board[i][1] == player && Board[i][2] == player) {
            return true;
        }
        if (Board[0][i] == player && Board[1][i] == player && Board[2][i] == player) {
            return true;
        }
    }

    // Checking Diagonal win conditions
    if (Board[0][0] == player && Board[1][1] == player && Board[2][2] == player) {
        return true;
    }

    if (Board[0][2] == player && Board[1][1] == player && Board[2][0] == player) {
        return true;
    }

    return false;
}

// False is X wins and true if O wins 
bool whoWon() {
    if (hasWon(Player::O)) {
        return true;
    }
    else {
        return false;
    }
}

// This one is the GOAT, the ultimate needle-mover, the head of the table, the tribal chief
// AI move generation
int minimax(Player player, int depth, int alpha, int beta, bool Maximizing) {
    if (hasWon(Player::O)) {
        return 10 - depth;
    }
    else if(hasWon(Player::X)){
        return depth - 10;
    }
    else if (isFull()) {
        return 0;
    }

    if (Maximizing) {
        int bestscore = INT_MIN;

        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (Board[i][j] == Player::None) {
                    Board[i][j] = Player::O;
                    int score = minimax(player, depth + 1, alpha, beta, false);
                    Board[i][j] = Player::None;
                    bestscore = max(bestscore, score);
                    alpha = max(alpha, score);
                    if (beta <= alpha) {
                        break;
                    }
                }
            }
        }
        return bestscore;
    }
    else {
        int bestscore = INT_MAX;

        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (Board[i][j] == Player::None) {
                    Board[i][j] = Player::X;
                    int score = minimax(player, depth + 1, alpha, beta, true);
                    Board[i][j] = Player::None;
                    bestscore = min(bestscore, score);
                    beta = min(beta, score);
                    if (beta <= alpha) {
                        break;
                    }
                }
            }
        }
        return bestscore;
    }
}

// AI move selection
Move bestMove(Player player) {
    int bestScore = INT_MIN;
    Move best;

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (Board[i][j] == Player::None) {
                Board[i][j] = player;
                int score = minimax(player, 0, INT_MIN, INT_MAX, false);
                Board[i][j] = Player::None;
                if (score > bestScore) {
                    bestScore = score;
                    best.col = j;
                    best.row = i;
                }
            }
        }
    }

    return best;
}

// Main function for players; works only through the Basic GUI and resets the game after a player wins
int main()
{
    initBoard();
    sf::Font font;
    if (!font.loadFromFile("BebasNeue-Regular.ttf")) {
        std::cerr << "Failed to load font" << std::endl;
        return 1;
    }

    sf::RenderWindow window(sf::VideoMode(300, 300), "Tic Tac Toe");
    Player current = Player::X;
    bool gameEnd = false;

    while (window.isOpen()) {
        sf::Event event;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::MouseButtonPressed && !gameEnd && current == Player::X) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                int x = mousePos.x / (window.getSize().x / 3);
                int y = mousePos.y / (window.getSize().y / 3);
                if (x >= 0 && x < 3 && y >= 0 && y < 3 && Board[y][x] == Player::None) {
                    Board[y][x] = current;
                    current = Player::O;
                }
            }
        }

        if (current == Player::O && !gameEnd) {
            Move best = bestMove(Player::O);
            Board[best.row][best.col] = Player::O;
            current = Player::X;
        }

        if (hasWon(Player::X)) {
            gameEnd = true;
        }
        else if (hasWon(Player::O)) {
            gameEnd = true;
        }
        else if (isFull()) {
            gameEnd = true;
        }

        window.clear();

        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                sf::RectangleShape square(sf::Vector2f(100, 100));
                square.setPosition(j * 100, i * 100);
                square.setOutlineThickness(2);
                square.setOutlineColor(sf::Color::Black);
                window.draw(square);

                if (Board[i][j] == Player::X) {
                    sf::Text xText("X", font, 50);
                    xText.setFillColor(sf::Color::Black);
                    xText.setPosition(j * 100 + 25, i * 100 + 10);
                    window.draw(xText);
                }
                else if (Board[i][j] == Player::O) {
                    sf::Text oText("O", font, 50);
                    oText.setFillColor(sf::Color::Black);
                    oText.setPosition(j * 100 + 25, i * 100 + 10);
                    window.draw(oText);
                }
            }
        }
        window.display();

        if (gameEnd) {
            sf::sleep(sf::seconds(2)); //pause before restarting
            initBoard();
            gameEnd = false;
            current = Player::X;
        }
    }
    return 0;
}

// This main has additional info on console but is not great for players
/*int main()
{
    initBoard();
    sf::Font font;
    if (!font.loadFromFile("C:\\Users\\abdul\\OneDrive - Institute of Business Administration\\Projects\\TicTacToe\\TicTacToe\\BebasNeue-Regular.ttf")) {
        std::cerr << "Failed to load font" << std::endl;
        return 1;
    }

    sf::RenderWindow window(sf::VideoMode(300, 300), "Tic Tac Toe");
    Player current = Player::X;
    bool gameEnd = false;

    while (window.isOpen()) {
        sf::Event event;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::MouseButtonPressed && !gameEnd && current == Player::X) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                int x = mousePos.x / (window.getSize().x / 3);
                int y = mousePos.y / (window.getSize().y / 3);
                std::cout << "Clicked at: " << x << ", " << y << std::endl;
                if (x >= 0 && x < 3 && y >= 0 && y < 3 && Board[y][x] == Player::None) {
                    Board[y][x] = current;
                    current = Player::O;
                }
            }
        }

        if (current == Player::O && !gameEnd) {
            Move best = bestMove(Player::O);
            Board[best.row][best.col] = Player::O;
            current = Player::X;
        }

        if (hasWon(Player::X)) {
            gameEnd = true;
            std::cout << "X wins!" << std::endl;
        }
        else if (hasWon(Player::O)) {
            gameEnd = true;
            std::cout << "O wins!" << std::endl;
        }
        else if (isFull()) {
            gameEnd = true;
            std::cout << "It's a draw!" << std::endl;
        }

        window.clear();

        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                sf::RectangleShape square(sf::Vector2f(100, 100));
                square.setPosition(j * 100, i * 100);
                square.setOutlineThickness(2);
                square.setOutlineColor(sf::Color::Black);
                window.draw(square);

                if (Board[i][j] == Player::X) {
                    sf::Text xText("X", font, 50);
                    xText.setFillColor(sf::Color::Black);
                    xText.setPosition(j * 100 + 25, i * 100 + 10);
                    window.draw(xText);
                }
                else if (Board[i][j] == Player::O) {
                    sf::Text oText("O", font, 50);
                    oText.setFillColor(sf::Color::Black);
                    oText.setPosition(j * 100 + 25, i * 100 + 10);
                    window.draw(oText);
                }
            }
        }
        window.display();

        if (gameEnd) {
            std::cout << "Press any key to continue..." << std::endl;
            system("pause >nul");
            initBoard();
            gameEnd = false;
            current = Player::X;
        }
    }

    return 0;
}*/