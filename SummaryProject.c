//
//  Automatic Extraction-based Text Summarization.
//

#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include <time.h>

char text[10000], text2[10000], tempWords[10000][100], words[10000][100], sentence[10000][1000], sentence2[10000][1000], sentWords[1000][1000][100];

int occurrences[10000], points[10000];


int separateWords( char text[10000], char tempWords[10000][100]); //Word separation using strtok function.

int frequencyWords(char tempWords[10000][100],char words[10000][100],int i,int occurrences[10000],int points[10000]);
//Occurrence of words.

int separateSentences( char text[10000], char sentence[10000][1000], char sentence2[10000][1000]); //Sentence separation.

void score(char words[10000][100],char sentWords[1000][1000][100],int points[10000],char sentence2[10000][1000], int uniqueWords,int sentenceCount, int wordCount);

int sentenceToWords(char sentence[10000][1000], char sentWords[1000][1000][100], int sentenceCount);


int main()
{
    clock_t tStart = clock();

    

    FILE *fp;
    fp = fopen("Test Cases/input_2.txt","r");

    fgets(text, 10000, (FILE*)fp);

    int s;
    for(s = 0; text[s]!='\0'; s++)
        text2[s] = text[s]; //text[] was modified by the strtok function used in separateWords.

    int sentenceCount = separateSentences(text2,sentence,sentence2);

    int wordCount = separateWords(text,tempWords);

    int uniqueWords = frequencyWords(tempWords,words,wordCount,occurrences,points);

    sentenceToWords(sentence, sentWords, sentenceCount);

    score(words,sentWords,points,sentence2,uniqueWords,sentenceCount,wordCount);

    printf("Text Successfully Summarized. Please open the 'output.txt' file to view.\n");
    printf("\n\nTime taken in summarizing: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);

    return 0;
}


int separateWords( char text[10000], char tempWords[10000][100])
{
    int i = 0;
    char *word = strtok(text," ");

    for (; word != NULL; word = strtok(NULL, ")! ,.-?(")) //Delimiters.
    {
        strcpy(tempWords[i],word); //"tempWords" array contains the separate words retrieved from the text.
        i++; //Word count.
    }

    return i;
}


int separateSentences( char text2[10000], char sentence[10000][1000], char sentence2[10000][1000])
{
    int i = 0;
    char *sent = strtok(text2,".");

    for (; sent != NULL; sent = strtok(NULL, ".")) //Delimiters.
    {
        strcpy(sentence[i],sent); //"sentence" array contains the separate sentences retrieved.
        strcpy(sentence2[i],sentence[i]);
        i++; //Sentence count.
    }

    return i;
}


int frequencyWords(char tempWords[10000][100], char words[10000][100], int wordCount, int occurrences[10000], int points[1000])
{
    int count, cmp;

    int i;
    for(i = 0; i < wordCount; i++)
    {
        count = 1;
        int j;
        for(j = i+1; j < wordCount; j++)
        {
            if(strlen(tempWords[i]) < 5)
                cmp = 4; else cmp = 5;
            if(strncasecmp(tempWords[i],tempWords[j],cmp)==0 && strcasecmp(tempWords[i]," ")!=0)
            {
                count++;
                strcpy(tempWords[j]," "); //Replaces word with space, word does not appear again in output.
            }
        }
        occurrences[i] = count;
    }

    printf("\nWord Count: %d\n\n", wordCount);

    int uniqueWords = 0;
    for(i = 0; i < wordCount; i++)
    {
        if(strcasecmp(tempWords[i]," ")==0); //Words that were replaced with spaces are not printed.
        else
        {
            strcpy(words[uniqueWords],tempWords[i]);
            points[uniqueWords] = occurrences[i];
            uniqueWords++;
        }
    }
    // [TEST CODE] - Displays word frequency count.
    /*for(int i = 0; i < uniqueWords; i++)
        printf("%15s %d\n",words[i], points[i]);*/

    return uniqueWords;
}


int sentenceToWords(char sentence[10000][1000], char sentWords[1000][1000][100], int sentenceCount)//Separates the sentences into words.
{
    int j = 0;
    int i;
    for(i = 0; i<sentenceCount; i++)
    {
        char *word = strtok(sentence[i]," ");

        for (; word != NULL; word = strtok(NULL, ")! ,-?(")) //Delimiters.
             {
                 strcpy(sentWords[i][j],word); //"new" array contains the separate words retrieved from the text.
                 j++; //Word count.
             }
    }

    return j;
}


void score(char words[10000][100],char sentWords[1000][1000][100],int points[10000],char sentence2[10000][1000],int uniqueWords,int sentenceCount,int wordCount)
{
    int sum[10000] = { 0 };

    char uselessWords[32][15] = /*Unimportant words*/
{"the","but","I","if","likewise","however","while","why","also","nor","any","for","if","a","an","in","on","at","it","he","she","so","from","to","and","or","his","her","of","my","as","we"};
    int i, j;

    for(i = 0; i < uniqueWords; i++)
        for(j = 0; j < uniqueWords; j++)
            if(strcasecmp(words[i],uselessWords[j])==0)
                points[i]=0;

    int cmp, k;
    for(i = 0; i < wordCount; i++)
        for(j = 0; j < wordCount; j++)
            for(k = 0; k < uniqueWords; k++)
            {
                if(strlen(sentWords[i][j]) < 5)
                    cmp = 4; else cmp = 5;
                if(strncasecmp(sentWords[i][j],words[k],cmp)==0)
                    sum[i]+= points[k];
            }

    int total = 0;
    for(i = 0; i < sentenceCount; i++)
        total += sum[i];

    //[TEST CODE] - Checks the score of each sentence.
    printf("Sentence score:  ");
    for(i = 0; i < sentenceCount; i++)
        printf("%d  ",sum[i]);

    FILE *f1;

    f1=fopen("Test Cases/output_2.txt","w");

    //fprintf(f1,"%s", 40+strlen("Summary")/2, "Summary", 40-strlen("Summary")/2, "");
    fprintf(f1,"Summary\n\n");

    fprintf(f1,"%s.",sentence2[0]); //Prints first Sentence. [Required]

    for(i = 1; i < sentenceCount; i++)
        if(sum[i] > (total/sentenceCount) * 1.25)
            fprintf(f1,"%s.\n",sentence2[i]);

    /*fprintf(f1,"%s.",sentence2[sentenceCount-1]);*/ //Prints last Sentence. [Optional]


    fclose(f1);
}
