#include <iostream>
#include <vector>
#include <unistd.h>  // For usleep
#include <termios.h> // For termios
#include <fcntl.h>
#include <cstdlib> // For random numbers

using namespace std;

void enableRawMode()
{
    termios term;
    tcgetattr(STDIN_FILENO, &term);
    term.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

void disableRawMode()
{
    termios term;
    tcgetattr(STDIN_FILENO, &term);
    term.c_lflag |= ICANON | ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

// klavye kontrolü
bool kbhit()
{
    int oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
    int ch = getchar();
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if (ch != EOF)
    {
        ungetc(ch, stdin);
        return true;
    }

    return false;
}

// yol. araba ve diğer arabaların çizimi
void drawRoad(vector<string> &road, int carPos, int &score, bool &gameOver)
{
    system("clear");

    // Check for collision
    if (road[road.size() - 2][carPos] == 'X')
    {
        gameOver = true;
        return;
    }

    // diğer araçları yoldan ilerleme
    for (int i = road.size() - 2; i > 0; --i)
    {
        road[i] = road[i - 1];
    }

    // yeni line diğer araçlar için
    road[0] = string(road[0].length(), ' ');
    if (rand() % 10 < 3)
    {
        road[0][rand() % (road[0].length() - 2)] = 'X';
    }

    // Diğer araçların çizimi
    for (const auto &line : road)
    {
        cout << line << endl;
    }

    // oyuncunun arabası
    cout << string(carPos, ' ') << "||" << endl;

    // score
    cout << "Score: " << score << endl;
}

// try again
bool tryAgain()
{
    char answer;
    cout << "You crashed! Would you like to try again? (y/n): ";
    cin >> answer;
    return answer == 'y' || answer == 'Y';
}

int main()
{
    const int roadWidth = 20;
    vector<string> road(10, string(roadWidth, ' '));
    int carPos = roadWidth / 2;
    int score = 0;
    bool gameOver = false;

    enableRawMode();
    srand(time(0));

    do
    {
        score = 0;
        gameOver = false;
        carPos = roadWidth / 2;

        while (!gameOver)
        {
            if (kbhit())
            {
                char c = getchar(); // Kullanıcıyı oku
                if (c == 'a')
                {
                    carPos = max(0, carPos - 1); // left
                }
                else if (c == 'd')
                {
                    carPos = min(roadWidth - 3, carPos + 1); // right
                }
                else if (c == 'q')
                {
                    gameOver = true; // Quit game
                }
            }

            drawRoad(road, carPos, score, gameOver);

            usleep(200000);
            score++;
        }

        if (gameOver)
        {
            cout << "Game Over! Your final score is: " << score << endl;
        }
    } while (tryAgain());

    disableRawMode();

    return 0;
    //bas gaza
}
