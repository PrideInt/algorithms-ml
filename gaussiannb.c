#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct type {
    struct word *words;
    int count;
};

struct word {
    char word[10];
    int probability;
};

struct splitword {
    char *word;
    struct splitword *next;
};

struct splitword *splitter(char *str) {
    struct splitword *split = malloc(sizeof(struct splitword));

    int size = 0;
    int size2 = 0;

    for (int i = 0; i < sizeof(str); i++) {
        if (str[i] == ' ') {
            size = i;
            size2 = sizeof(str) - i;
            break;
        }
    }
    char a[size];
    char b[size2];

    for (int i = 0; i < size; i++) {
        a[i] = str[i];
    }

    int j = 0;
    for (int i = size + 1; i < sizeof(str) + 1; i++) {
        b[j] = str[i];
        j++;
    }
    // printf("B: %s\n", b);
    split->word = a;
    split->next = malloc(sizeof(struct splitword));
    split->next->word = b;
    split->next->next = NULL;
    return split;
}

float posterior(struct type *types, char *A, char *B) {
    float likelihood;
    float prior;
    float p;

    struct splitword *split = splitter(A);
    // printf("A: %s\n", split->word);
    // printf("B: %s\n", split->next->word);

    if (strcmp(B, "spam") == 0) {
        p = 0.2;
    } else if (strcmp(B, "ham") == 0) {
        p = 0.8;
    } else {
        return -1;
    }

    for (int i = 0; i < 9; i++) {
        if (strcmp(A, types[0].words[i].word) == 0) {
            likelihood = (float)types[0].words[i].probability / (float)types[0].count;
            // prior += (float)types[0].words[i].probability;
        }
        if (strcmp(A, types[1].words[i].word) == 0) {
            // prior += (float)types[1].words[i].probability;
            break;
        }
    }
    // printf("Likelihood: %f\n", likelihood);
    // printf("Prior: %f\n", prior);
    return likelihood * prior * p;
}

int main() {
    // A very hardcoded thing; I just need to see for now
    printf("--- Simple Spam/Ham Naive Bayes: ---\n");

    FILE *file = fopen("./datasets/nb.csv", "r");

    struct type types[2]; // type[0] = spam, type[1] = ham
    struct word spam[9];
    struct word ham[9];

    int scan = 0;
    int i = 0;

    while (scan != EOF) {
        if (i > 8) {
            break;
        }
        char word[10];

        scan = fscanf(file, "%[^,],%d,%d\n", &word, &spam[i].probability, &ham[i].probability);

        strcpy(spam[i].word, word);
        strcpy(ham[i].word, word);
        i++;
    }
    /*
    for (int i = 0; i < 9; i++) {
        printf("%d, %s, %d, %d\n", i, spam[i].word, spam[i].probability, ham[i].probability);
    }
    */
    types[0].words = spam;

    int spamCount = 0;
    for (int i = 0; i < 9; i++) {
        spamCount += spam[i].probability;
    }
    types[0].count = spamCount;

    //

    types[1].words = ham;

    int hamCount = 0;
    for (int i = 0; i < 9; i++) {
        hamCount += ham[i].probability;
    }
    types[1].count = hamCount;

    /*
    printf("Spam:\n Count: %d\n", types[0].count);
    for (int i = 0; i < 9; i++) {
        printf("Word: %s, Prob: %d\n", types[0].words[i].word, types[0].words[i].probability);
    }
    */
    printf("%f", posterior(types, "Hello Now", "spam"));

    fclose(file);

    return 0;
}