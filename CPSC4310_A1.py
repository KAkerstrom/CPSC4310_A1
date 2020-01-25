import nltk
from nltk.corpus import brown
from nltk.corpus import stopwords
from nltk.stem import PorterStemmer
from nltk.stem import WordNetLemmatizer

print("Checking for NLTK modules...")
nltk.download("stopwords")
nltk.download("brown")
nltk.download("wordnet")
nltk.download("stopwords")

lemmatizer = WordNetLemmatizer()
porter = PorterStemmer()
stopword_list = set(stopwords.words('english'))

def Lemmatize(tagged_word):
    pos = "n"
    pos_dict = {"JJ": 'a', "RB": 'r', "VB": 'v'}
    for p in pos_dict:
        if tagged_word[1].startswith(p):
            pos = pos_dict[p]
            break
    return lemmatizer.lemmatize(tagged_word[0], pos)

class WordList:
    def __init__(self):
        pass

    words = set()
    types = set()

    def AddTuple(self, tup):
        self.words.add(tup[0])
        self.types.add(tup[1])

    def GetWordAmt(self):
        return len(self.words)
    
    def GetTypeAmt(self):
        return len(self.types)

Cats = {}
Cats["stopwords"] = {} # with stopwords
Cats[""] = {} # without stopwords
Cats["lemma"] = {} # without stopwords, with lemmatization
Cats["stem"] = {} # without stopwords, with stemming

total_words = 0
c_index = 0
c_total = len(brown.categories())
print("\nCategories Processed:")
for c in brown.categories():
    c_index += 1
    print("\t[{0}/{1}] ".format(c_index, c_total), c, sep = "")
    for cat in Cats:
        Cats[cat][c] = WordList()
    for w in brown.tagged_words(categories = c):
        Cats["stopwords"][c].AddTuple(w)
        if w not in stopword_list:
            Cats[""][c].AddTuple(w)
            Cats["lemma"][c].AddTuple((Lemmatize(w), w[1]))
            Cats["stem"][c].AddTuple((porter.stem(w[0]), w[1]))
    if total_words > 0:
        quit()
    total_words += Cats["stopwords"][c].GetWordAmt()


print('\n')
print("\tCategory            \tWords\tTypes")
print("\t--------            \t-----\t-----")
for c in brown.categories():
    print("\t", c.ljust(20, ' '), '\t', Cats[""][c].GetWordAmt(), '\t', Cats[""][c].GetTypeAmt(), sep = "")


# Total words selected
print("\n\tTotal words selected:")
print("\t", total_words, sep = "")