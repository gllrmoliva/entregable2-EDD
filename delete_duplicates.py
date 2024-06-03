import pandas as pd

filename = 'universities_followers'

# Leer el archivo CSV
df = pd.read_csv(filename + ".csv")

# Eliminar los duplicados basados en 'user_name', manteniendo el primer registro encontrado
df_unique = df.drop_duplicates(subset='user_name', keep='first')

# Guardar el DataFrame resultante en un nuevo archivo CSV
df_unique.to_csv(filename + '_without_duplicates.csv', index=False)
