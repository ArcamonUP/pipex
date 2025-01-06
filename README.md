Pipex !

S'utilise de la maniere suivante : 

- make
- ./pipex infile "cmd 1" "cmd 2" "cmd 3" ... outfile (equivaut a < infile cmd 1 cmd 2 cmd 3 outfile > en shell)
- ./pipex here_doc LIMITER "cmd1" "cmd2" ... outfile (equivaut a << limiter cmd 1 cmd 2 cmd 3 outfile >> en shell)
