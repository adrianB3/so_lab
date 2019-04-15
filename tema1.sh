#!/bin/bash
function menu()
{
    echo '1.Iesire'
    echo '2.Informatii utilizator'
    echo '3.Logged in users'
    echo '4.Afisare shell-uri instalate'
    echo '5.Afisare nume login, nume si home dir utilizatori'
    echo '6.Afisare informatii sistem operare'
    echo '7.Afisare tip procesor si frecventa'
    echo '8.Afisare memorie totala si memorie libera'
    echo '9.Afisare si cautare in loguri'
    echo -e '\t 1.Fisiere log curente'
    echo -e '\t 2.Selectare fisier log si cautare text'
}

function current_user_info()
{
    echo 'Nume login: ' $USER
    echo 'Home dir: ' $HOME
    echo 'Cale curenta: ' $(pwd)
    echo 'Tip shell utilizat: ' $SHELL
}

function loggedin_users()
{
    echo 'Nume utilizatori logati: ' $(who | awk '{print $1}')
    echo 'Nr utilizatori logati: ' $(who | wc -l)
}

function shells()
{
    cat /etc/shells
}

function all_users()
{
    awk -F : '{print $1, $5, $7}' /etc/passwd
}

function so()
{
    echo 'Sistem operare: ' $(uname -o)
    echo 'Nume calculator: ' $(uname -n)
    echo 'Nr iden kernel: ' $(uname -r)
}

function proc_freq()
{
    cat /proc/cpuinfo | grep 'model name' | uniq
    cat /proc/cpuinfo | grep 'cpu MHz' | uniq
}

function proc_mem()
{
    cat /proc/meminfo | grep 'MemTotal' | uniq
    cat /proc/meminfo | grep 'MemFree' | uniq
}

menu
read -p 'Optiune: ' opt
while [ $opt -ne 1 ] 
do
    menu
    case $opt in
        1) break
        ;;
        2) current_user_info
        ;;
        3) loggedin_users
        ;;
        4) shells
        ;;
        5) all_users
        ;;
        6) so
        ;;
        7) proc_freq
        ;;
        8) proc_mem
        ;;
        9) read -p 'Sec opt: ' opt2
            case $opt2 in
                1) #ls /var/log/*.log 
                    ls /var/log/ | grep \.log                  
                ;;
                2) 
                ls /var/log/ | grep \.log
                read -p "Select file: " file 
                read -p "Search for: " text 
                
                files=$(ls /var/log/ | grep $file) 
                echo $files
                for VAR in $files
                do
                    zgrep $text /var/log/$VAR
                done                
                ;;
                *) echo Not an option dude
                ;;
            esac
        ;;
        *) echo Not an option dude
        ;;
    esac
    echo
    menu
    read -p 'Optiune: ' opt
    echo
done
