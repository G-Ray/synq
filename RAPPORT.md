# synq - Rapport

Synqronize your files locally or with a server.

Fonctionnalités implémentées
-------
Les lignes de commandes sont "parsées" de la même manière que dans le sujet du projet.

L'option -d est implémentée.

La synchronisation locale et distante fonctionne. Les fichiers conservent leurs droits de modifications et leur date de modification.

Les suppressions de fichiers sont détectées et affichées à l'utilisateur. Cependant, ces modifications ne sont pas répercutées. On pourrait facilement ajouter une option pour activer la synchronisation automatique de la suppression des fichiers, le code étant déjà présent du côté client et du côté serveur.

Les données transmises en TCP sont chiffrées par OpenSSL, aussi bien les TLVs que les données transmises. Le chiffrement peut avoir un impact significatif sur un réseau à haute vitesse.
Il suffit de générer un certificat du côté du serveur. Cependant, l'identité des clients n'est pas vérifiée.

Une fonction de hashage des fichiers en SHA1 a été implémenté, et son résultat à été comparé avec succès au résultat de la commande "sha1sum fichier" dans un terminal. Cependant, le hash n'est pas intégré à un TLV.

Précisions d'implémentation
-------
Il y a un certain nombre de fonctions partagées entre le client et le serveur dans le dossier "common".

Un parcours récursif du répertoire à synchroniser est éxécuté une seule fois pas le client et le serveur. Le client récupère la liste (chainée) complète des fichiers et dossiers présents de part et d'autres. Une fonction de comparaison de ses deux listes nous donne un nouvelle liste des fichiers à synchroniser.

La liste des fichiers du client et du serveur sont sérialisées respectivement dans les fichiers "local_files.synq" et "remote_files.synq".

Le serveur vérifie la validité du TLV du connexion du client, puis répond à toutes les demandes du client jusqu'à déconnexion de celui-ci. (ou de l'envoi d'un message invalide).

Les valeurs de retour des fonctions ne sont pas systématiquement vérifiés...
