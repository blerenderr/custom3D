#include "ui.h"

void UIElement::setNumber(int num) {
    this->number = num;
}
void UIElement::drawOneDigit(int n, int digitIndex) {
    for(int i = 0; i < 6; i++) {
        for(int j = 0; j < 6; j++) {
            if(DISP_NUM[n][i][j]) {
                SDL_SetRenderDrawColor(pRenderer, 0,255,0,0);
                SDL_RenderDrawPoint(pRenderer, j+xPos+digitIndex*7, i+yPos);
            }
        }
    }
}
void UIElement::draw() {
    int n = number;
    std::vector<int> digits;
    while(n != 0) {
        digits.insert(digits.begin(), n % 10);
        n /= 10;
    }

    for(int i = 0; i < digits.size(); i++) {
        drawOneDigit(digits.at(i), i);
    }

}