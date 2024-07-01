SELECT title, ratings.rating FROM movies JOIN ratings ON movies.id = ratings.movie_id WHERE year = 2010 ORDER BY ratings.rating DESC, title ASC
