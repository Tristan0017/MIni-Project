#include <BasicLinearAlgebra.h>
using namespace BLA;
#include <Arduino.h>
#include <Wire.h>
#include "KF.h"
KF myKF;
#include "Meas.h" 
Meas Sensors;

const int buttonPin {2};
const int choiceButton1 {3};
const int choiceButton2 {4};
const int choiceButton3 {5};
const int arraySize {5};
const int numberOfRounds {3};
const int buzzer {8};
int delayFacit[arraySize];
int spillerDelay {0};
int spillerDelayOffset {0};
int gameState {0};
int roundCount;
int StarPower {0};
int UsedStarPower {0};

typedef struct delayProfil
{
  int delayPlayer[arraySize];
  int delayForskel[arraySize];
  int delaySum[numberOfRounds];
  int StarPowerArray[numberOfRounds] {0};
  float delaySnit;
  int LED;
};

delayProfil player1;
delayProfil player2;
delayProfil player3;

void setup() 
{
    Serial.begin(115200);
    while (!Serial);
    Sensors.SensorSetup();  
    pinMode(choiceButton1,INPUT);
    pinMode(choiceButton2,INPUT);
    pinMode(choiceButton3,INPUT);
    pinMode(buttonPin,INPUT);
    player1.LED = 10;
    player2.LED = 11;
    player3.LED = 12;
}

void loop()
{
    gamePlay();
}

unsigned int chooseGame()
    {  //This function is used initially to decide how many players are included.
    Serial.println("Chose number of players");
    while ((digitalRead(choiceButton1)==LOW)&&(digitalRead(choiceButton2)==LOW)&&(digitalRead(choiceButton3)==LOW))
    {
    digitalRead(choiceButton1);
    digitalRead(choiceButton2);
    digitalRead(choiceButton3);
    }
    if(digitalRead(choiceButton1)==HIGH){
    gameState=1;
    }
    if(digitalRead(choiceButton2)==HIGH){
    gameState=2;
    }
    if(digitalRead(choiceButton3)==HIGH){
    gameState=3;
    }
    return(gameState);
    }

void gamePlay()
    { //This is the main loop of the program. The switch case is sensitive to the variable used, to defined the amount of players.
    switch(chooseGame()){
    case 1:
    roundCount=0;
    for(int i=0;i<numberOfRounds;i++){
    genererFacit(delayFacit);
    spilStart();
    spilLyd(delayFacit);
    afventStart(player1.LED);
    spillerFacit(player1.delayPlayer, player1.StarPowerArray);
    evalFacit(delayFacit, player1.delayPlayer, player1.delayForskel, player1.delaySum, player1.StarPowerArray);
    roundCount=roundCount+1;
    }
    Serial.println("Game Over, evaluating results .. ");
    Serial.println(" ");
    compare1(&player1.delaySnit, player1.delaySum);
    break;

    case 2:
    roundCount=0;
    for(int i=0;i<numberOfRounds;i++){
    genererFacit(delayFacit);
    spilStart();
    spilLyd(delayFacit);
    afventStart(player1.LED);
    spillerFacit(player1.delayPlayer, player1.StarPowerArray);
    spilLyd(delayFacit);
    afventStart(player2.LED);
    spillerFacit(player2.delayPlayer, player2.StarPowerArray);
    evalFacit(delayFacit, player1.delayPlayer, player1.delayForskel, player1.delaySum, player1.StarPowerArray);
    evalFacit(delayFacit, player2.delayPlayer, player2.delayForskel, player2.delaySum, player2.StarPowerArray);
    roundCount=roundCount+1;
    }
    Serial.println("Game Over, evaluating results .. ");
    Serial.println(" ");
    compare2(&player1.delaySnit, player1.delaySum, &player2.delaySnit, player2.delaySum);
    break;

    case 3:
    roundCount=0;
    for(int i=0;i<numberOfRounds;i++){
    genererFacit(delayFacit);
    spilStart();
    spilLyd(delayFacit);
    afventStart(player1.LED);
    spillerFacit(player1.delayPlayer, player1.StarPowerArray);
    spilLyd(delayFacit);
    afventStart(player2.LED);
    spillerFacit(player2.delayPlayer, player2.StarPowerArray);
    spilLyd(delayFacit);
    afventStart(player3.LED);
    spillerFacit(player3.delayPlayer, player3.StarPowerArray);
    evalFacit(delayFacit, player1.delayPlayer, player1.delayForskel, player1.delaySum, player1.StarPowerArray);
    evalFacit(delayFacit, player2.delayPlayer, player2.delayForskel, player2.delaySum, player2.StarPowerArray);
    evalFacit(delayFacit, player3.delayPlayer, player3.delayForskel, player3.delaySum, player3.StarPowerArray);
    roundCount=roundCount+1;
    }
    Serial.println("Game Over, evaluating results .. ");
    Serial.println(" ");
    compare3(&player1.delaySnit, player1.delaySum, &player2.delaySnit, player2.delaySum, &player3.delaySnit, player3.delaySum);
    break;  

    }
    }

void spilStart()
    { // This function starts the game, and is used in the beginning of every round.
    Serial.println(" ");
    Serial.println("Push Game Button");
    Serial.println(" ");
    while(digitalRead(buttonPin)==LOW)
    {
    digitalWrite(player1.LED,HIGH);
    digitalWrite(player2.LED,HIGH);
    digitalWrite(player3.LED,HIGH);
    }
    delay(1000);
    digitalWrite(player1.LED,LOW);
    digitalWrite(player2.LED,LOW);
    digitalWrite(player3.LED,LOW);
    }

void genererFacit(int arr[])
    { //Here the true delays between buzzing sounds are generated.
    randomSeed(analogRead(1)); 
    for(int d=0;d<arraySize;d++){
    arr[d]= (random(700)+100);
    }
    }

void spilLyd(int arr[])
    { // This functions plays the tone distrubted with the chosen spacings. 
    tone(buzzer,2000);
    delay(100);
    noTone(buzzer);
    for(int c=0;c<arraySize;c++){
    delay(arr[c]);
    tone(buzzer, 1500);
    delay(50);
    noTone(buzzer); 
    Serial.print("Delay ");
    Serial.print(c+1);
    Serial.print("  ");
    Serial.println(arr[c]); 
    }
    Serial.println(" ");
    }

void afventStart(int led)
    {  //This function is called, whenever it is time to switch players.
    Serial.print("Waiting for player ");
    if(led==10)
    {
     Serial.print("1");   
    }
    if(led==11)
    {
     Serial.print("2");   
    }
    if(led==12)
    {
     Serial.print("3");   
    }

    Serial.print(", press the game button to begin.");
    Serial.println(" ");
    while(digitalRead(buttonPin)==LOW)
    {
    digitalWrite(led,HIGH);
    }
    delay(1000);
    digitalWrite(led,LOW);
    }

void spillerFacit(int arr[], int star[])
    {  //Here the player attempts to mimic the delays between sounds. Also, it is in this function the KF filter is run, to determine if the player will get their penalty halved.
    tone(buzzer,2000);
    delay(100);
    noTone(buzzer);  
    for(int a=0;a<arraySize;a++){
    spillerDelayOffset=millis();
        while(digitalRead(buttonPin) == LOW)
        {
            Sensors.Gyroscope();  //Here the angular velocity is obtained.
            Sensors.Accelerometer(); //Here the angular position from the Accelerometer is obtained.
            Sensors.Magnetometer(); //Here the angular position from the Magnetometer is obtained.
            BLA::Matrix<2,1> Z = Sensors.GetMeasurements(); // The angular velocity and the average of the angular positions obtained are saved as Z
            myKF.KFPredict();
            myKF.KFUpdate(Z); // The Z vector is used in the update step of the KF.
            if(myKF.KFGetY()>45.0) // Check if the attitude is higher than 45 degrees. If yes, star power is activated for the round.
            {
                star[roundCount]=1;
            }
            delay(16);
        }
    spillerDelay=millis()-spillerDelayOffset;
    tone(buzzer, 1500);
    delay(50);
    noTone(buzzer);
    arr[a]=spillerDelay;
    Serial.print("Player delay in round:");
    Serial.print(a+1);
    Serial.print(" is ");
    Serial.println(arr[a]);
    delay(100);
    }
    Serial.println(" ");
    }

void evalFacit(int facit[], int arr[], int diff[], int sum[], int starpower[])
    { //Here the players turn is evaluated (after every round), and the facit is deployed. 
    Serial.println("Evaluating round .. ");
    Serial.println(" ");
    for(int a=0;a<arraySize;a++){
    diff[a]=sqrt((pow((facit[a]-arr[a]),2)));    
    Serial.print("Delay difference in round: ");
    Serial.print(a+1);
    Serial.print("  ");
    Serial.println(diff[a]);
    }
    sum[roundCount]=0;
    for(int a=0;a<arraySize;a++)
    {
    sum[roundCount]=sum[roundCount]+diff[a];  
    }
    if(starpower[roundCount]==1)
    {
        if(roundCount == 0)   // These checks determine, if the star power has already been used. 
        {
            sum[roundCount]=sum[roundCount]/2;
            Serial.println("Star power activated");
        }
        else if(roundCount == 1 && starpower[roundCount-1]!=1)
        {
            sum[roundCount]=sum[roundCount]/2;
            Serial.println("Star power activated");
        }
        else if(roundCount == 2 && starpower[roundCount-1]!=1 && starpower[roundCount-2]!=1)
        {
            sum[roundCount]=sum[roundCount]/2;
            Serial.println("Star power activated");
        }
        else
        {
            Serial.println("No cheating!");
        }
    }
    Serial.println(" ");
    Serial.print("Sum of player delay: ");
    Serial.println(sum[roundCount]);
    Serial.println(" ");
    }

void compare1(float *snit, int delaySum[])
    {  // This function is used to compute the mean deviation for one player in the end of the game.
    float sumTotal=0;
    for(int a=0;a<numberOfRounds;a++){
    sumTotal=delaySum[a];
    }
    *snit=sumTotal/3;
    Serial.println(" ");
    Serial.print("Average of difference in each round is ");
    Serial.println(*snit);
    Serial.println(" ");
    }

void compare2(float *snit1, int delaySum1[], float *snit2, int delaySum2[])
    {  //This function obtains the average delay devation for two players, after which a winner is found.
    float sumTotal=0;
    for(int a=0;a<numberOfRounds;a++){
    sumTotal=delaySum1[a];
    }
    *snit1=sumTotal/3;
    sumTotal=0;
    for(int a=0;a<numberOfRounds;a++){
    sumTotal=delaySum2[a];
    }
    *snit2=sumTotal/3;
    if(*snit1==*snit2){
    Serial.println("It's a tie!"); 
    }
    if(*snit1<*snit2){
    Serial.print("Average of difference in each round for player 1 is ");
    Serial.println(*snit1);
    Serial.print("Average of difference in each round for player 2 is ");
    Serial.println(*snit2);
    Serial.println(" ");
    Serial.println("Player 1 wins!");
    Serial.println(" ");
    }
    if(*snit1>*snit2){
    Serial.print("Average of difference in each round for player 1 is ");
    Serial.println(*snit1);
    Serial.print("Average of difference in each round for player 2 is ");
    Serial.println(*snit2);
    Serial.println(" ");
    Serial.println("Player 2 wins!");
    Serial.println(" ");
    }
    }

void compare3(float *snit1, int delaySum1[],float *snit2, int delaySum2[], float *snit3, int delaySum3[])
    { //This function obtains the average delay devation for three players, after which a winner is found.
    float sumTotal=0;
    for(int a=0;a<numberOfRounds;a++){
    sumTotal=delaySum1[a];
    }
    *snit1=sumTotal/3;
    sumTotal=0;
    for(int a=0;a<numberOfRounds;a++){
    sumTotal=delaySum2[a];
    }
    *snit2=sumTotal/3;
    sumTotal=0;
    for(int a=0;a<numberOfRounds;a++){
    sumTotal=delaySum3[a];
    }
    *snit3=sumTotal/3;
    if(*snit1==*snit2==*snit3){
    Serial.println("It's a tie!"); 
    }
    if((*snit1<*snit2)&&(*snit1<*snit3)){
    Serial.print("Average of difference in each round for player 1 is ");
    Serial.println(*snit1);
    Serial.print("Average of difference in each round for player 2 is ");
    Serial.println(*snit2);
    Serial.print("Average of difference in each round for player 3 is ");
    Serial.println(*snit3);
    Serial.println(" ");
    Serial.println("Player 1 wins!");
    Serial.println(" ");
    }
    if((*snit1>*snit2)&&(*snit3>*snit2)){
    Serial.print("Average of difference in each round for player 1 is ");
    Serial.println(*snit1);
    Serial.print("Average of difference in each round for player 2 is ");
    Serial.println(*snit2);
    Serial.print("Average of difference in each round for player 3 is ");
    Serial.println(*snit3);
    Serial.println(" ");
    Serial.println("Player 2 wins!");
    Serial.println(" ");
    }
    if((*snit1>*snit3)&&(*snit2>*snit3)){
    Serial.print("Average of difference in each round for player 1 is ");
    Serial.println(*snit1);
    Serial.print("Average of difference in each round for player 2 is ");
    Serial.println(*snit2);
    Serial.print("Average of difference in each round for player 3 is ");
    Serial.println(*snit3);
    Serial.println(" ");
    Serial.println("Player 3 wins!");
    Serial.println(" ");
    }
}
