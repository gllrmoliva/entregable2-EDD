import csv
import random
import string
from datetime import datetime, timedelta

# los nombres se los pedí a chat gpt, viva la IA
names = [
        "liam", "noah", "oliver", "elijah", "william", "james", "benjamin", "lucas", "henry", "alexander",
        "mason", "michael", "ethan", "daniel", "jacob", "logan", "jackson", "levi", "sebastian", "mateo",
        "jack", "owen", "theodore", "aiden", "samuel", "joseph", "john", "david", "wyatt", "matthew",
        "luke", "asher", "carter", "julian", "grayson", "leo", "jayden", "gabriel", "isaac", "lincoln",
        "anthony", "hudson", "dylan", "ezra", "thomas", "charles", "christopher", "jaxon", "maverick", "josiah",
        "isaiah", "andrew", "elias", "joshua", "nathan", "caleb", "ryan", "adrian", "miles", "eli",
        "nolan", "christian", "aaron", "cameron", "ezekiel", "colton", "luca", "landon", "hunter", "jonathan",
        "santiago", "axel", "easton", "cooper", "jeremiah", "angel", "roman", "connor", "jameson", "robert",
        "greyson", "jordan", "ian", "carson", "jaxson", "leonardo", "nicholas", "dominic", "austin", "everett",
        "brooks", "xavier", "kai", "jose", "parker", "adam", "jace", "wesley", "kayden", "silas"
    ]

last_names = [
        "smith", "johnson", "williams", "brown", "jones", "garcia", "miller", "davis", "rodriguez", "martinez",
        "hernandez", "lopez", "gonzalez", "wilson", "anderson", "thomas", "taylor", "moore", "jackson", "martin",
        "lee", "perez", "thompson", "white", "harris", "sanchez", "clark", "ramirez", "lewis", "robinson",
        "walker", "young", "allen", "king", "wright", "scott", "torres", "nguyen", "hill", "flores",
        "green", "adams", "nelson", "baker", "hall", "rivera", "campbell", "mitchell", "carter", "roberts",
        "gomez", "phillips", "evans", "turner", "diaz", "parker", "cruz", "edwards", "collins", "reyes",
        "stewart", "morris", "morales", "murphy", "cook", "rogers", "gutierrez", "ortiz", "morgan", "cooper",
        "peterson", "bailey", "reed", "kelly", "howard", "ramos", "kim", "cox", "ward", "richardson",
        "watson", "brooks", "chavez", "wood", "james", "bennett", "gray", "mendoza", "ruiz", "hughes",
        "price", "alvarez", "castillo", "sanderson", "patel", "myers", "long", "ross", "foster", "jimenez"
    ]


def generate_random_date(start_year=2000, end_year=2024):
    # Genera fechas aleatorias en un rango dado
    start_date = datetime(start_year, 1, 1)
    end_date = datetime(end_year, 12, 31)
    random_date = start_date + timedelta(days=random.randint(0, (end_date - start_date).days))
    return random_date.strftime('%a %b %d %H:%M:%S +0000 %Y')

def generate_random_username():
    # Genera nombres de usuario aleatorios de la forma nombre - apellido - numero
    first_name = random.choice(names)
    last_name = random.choice(last_names)
    number = random.randint(0, 9999)
    return f"{first_name}{last_name}{number}"

def generate_user_id():
    # Genera un numero aleatorio(64 bits)
    return random.randint(0, (1 << 63) - 1)

def generate_random_data(universities, num_entries):
    # A partir de todo lo anterior genera "usuarios" en forma de listas
    data = []
    for _ in range(num_entries):
        university = random.choice(universities)
        user_id = generate_user_id()
        user_name = generate_random_username()
        number_tweets = random.randint(1, 10000)
        friends_count = random.randint(0, 5000)
        followers_count = random.randint(0, 100000)
        created_at = generate_random_date()

        data.append([university, user_id, user_name, number_tweets, friends_count, followers_count, created_at])
    return data

def write_to_csv(filename, data):
    # crea el archivo CSV con los datos generamos anteriormente
    headers = ['university', 'user_id', 'user_name', 'number_tweets', 'friends_count', 'followers_count', 'created_at']
    with open(filename, mode='w', newline='') as file:
        writer = csv.writer(file)
        writer.writerow(headers)
        writer.writerows(data)

if __name__ == "__main__":
    universities = ['udec_cl', 'fake_cl', 'harvard_cl', 'chile_cl', 'noestudie_cl']
    num_entries = 30000 

    random_data = generate_random_data(universities, num_entries)
    write_to_csv('fake_data.csv', random_data)