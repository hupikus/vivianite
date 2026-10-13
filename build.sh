#!/usr/bin/env bash
:
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"

NAME="vivianite"

PROFILES_DIR_NAME="build_profiles"
PROFILES_DIR="$SCRIPT_DIR/$PROFILES_DIR_NAME"

BUILD_PATH=$(cat "$PROFILES_DIR/build_dir.txt" 2>/dev/null)
BUILD_PATH=${BUILD_PATH:-builds}


XDG_LOCAL_INSTALL_HOME=${XDG_DATA_HOME:-"/home/$USER/.local/share"}
XDG_LOCAL_APPS_DIR="$XDG_LOCAL_INSTALL_HOME/applications"
INSTALL_DIR="$XDG_LOCAL_INSTALL_HOME/vivianite"

# Build name
NAME="vivianite"

SELECTED=0
RESP=""
EDIT_RESP=""

# Parse flags

JOBS=0
if [[ "$*" == *"--jobs"* ]]; then
    JOBS=1
fi

CLEAN=0
if [[ "$*" == *"--clean"* ]]; then
    CLEAN=1
fi

DISTCLEAN=0
if [[ "$*" == *"--clean"* ]]; then
    DISTCLEAN=1
fi

EDIT=0
if [[ "$*" == *"--edit"* ]]; then
    EDIT=1
fi

DELETE=0
if [[ "$*" == *"--delete"* ]]; then
    DELETE=1
fi

NO_RECONFIGURE=0
if [[ "$*" == *"--no-configure"* ]]; then
    NO_RECONFIGURE=1
fi

FORCE_RECONFIGURE=0
if [[ "$*" == *"--no-configure"* ]]; then
    NO_RECONFIGURE=0
    FORCE_RECONFIGURE=1
fi

CREATE_SHORTCUT=0
if [[ "$*" == *"--shortcut"* ]]; then
    CREATE_SHORTCUT=1
fi

INSTALL=0
if [[ "$*" == *"--install"* ]]; then
    INSTALL=1
    CREATE_SHORTCUT=1
fi

UNINSTALL=0
if [[ "$*" == *"--uninstall"* ]]; then
    UNINSTALL=1
    INSTALL=0
fi

RUN=0
if [[ "$*" == *"--run"* ]]; then
    RUN=1
fi

if [[ "$*" == *"--help"* ]]; then
    printf "Help\n"

    printf "\t--help                Print help message and exit\n"
    printf "\t--vars                Print all avaliable build profile variables and exit\n"
    printf "\t--flags               Print all avaliable build flags and exit\n\n"

    printf "\t--no-reconfigure      Skip Make reconfigure and configure (if set up)\n"
    printf "\t--force-reconfigure   Force automake to regenerate configure script even if disabled\n"
    printf "\t--clean               Run a clean build\n"
    printf "\t--distclean           Run a distclean build\n"
    printf "\t--jobs                Compile parallelly (specifying number of jobs not yet avaliable)\n\n"

    printf "\t--shortcut            Create application shortcut in script's folder\n\n"
    printf "\t--run                 Run the build after it's done\n"
    printf "\t--install             Install the application\n\n"
    # printf "\t--uninstall           Uninstall the application\n\n"

    printf "\t--edit                Edit existing build\n"
    printf "\t--delete              Delete a build\n"
    printf "\t--reset               Remove all build profiles\n"

    exit 0
fi

if [[ "$*" == *"--vars"* ]]; then
    printf "NAME BUILD_PATH INSTALL_DIR\n"
    exit 0
fi

if [[ "$*" == *"--flags"* ]]; then
    printf "CLEAN DISTCLEAN JOBS DEBUG NO_CONFIGURE FORCE_RECONFIGURE CREATE_SHORTCUT RUN INSTALL\n"
    exit 0
fi

if [[ "$*" == *"--reset"* ]]; then
    rm "$PROFILES_DIR/*"
fi

# Profile directory
if [ ! -d "$PROFILES_DIR" ]; then
    mkdir -p "$PROFILES_DIR" || {
        echo "Failed to create profile directory." >&2
        exit 1
    }


    if [[ "$DELETE" == 1 ]]; then
        echo "Nothing to delete."
        exit 1
    else
        echo "Creating a new build profile."
        RESP="N"
    fi
else

    set --
    for f in "$PROFILES_DIR"/*.profile; do
        [ -f "$f" ] || continue
        set -- "$@" "$f"
    done

    PROFILE_COUNT=$#

    if [ "$PROFILE_COUNT" -gt 0 ]; then
        if [[ "$EDIT" == 1 ]]; then
            echo "Select a build to edit:"
        else
            if [[ "$DELETE" == 1 ]]; then
                echo "Select a build to delete:"
            else
                echo "Select appropriate build:"
            fi
        fi
        n=1

        for f do
            profile_name=${f##*/}
            profile_name=${profile_name%.profile}
            printf '[%s] %s\n' "$n" "$profile_name"
            n=$((n + 1))
        done

        if [[ "$EDIT" == 0 ]]; then
            if [[ "$DELETE" == 1 ]]; then
                printf "[N] Delete a new build\n"
            else
                printf "[N] Create a new build\n"
            fi
        fi
        #printf "[E] Edit a build\n"
        printf '> '
        IFS= read -r EDIT_RESP || EDIT_RESP=


        if [[ "$EDIT" == 1 ]]; then
            RESP="N"
        else
            RESP="$EDIT_RESP"
        fi
    else
        echo "No existing profiles found."
        RESP="N"
    fi
fi



if [[ "$DELETE" == 0 ]]; then
    NEW_ITEM_INDEX=$[PROFILE_COUNT+1]
    if [[ "$RESP" -eq "$NEW_ITEM_INDEX" ]]; then
        RESP=N
        echo "Creating a new build."
    fi
fi

# Process profile selection or create one
APPEND_FLAGS=
case "$RESP" in
    [Nn])

        FLAGS_EDIT_READ=
        profile_file=
        if [[ "$EDIT" == 1 ]]; then
            n=1
            for f in "$PROFILES_DIR"/*.profile; do
                if [ "$n" -eq "$EDIT_RESP" ]; then
                    profile_file="$f"
                    APP_PATH="$BUILD_PATH/$NAME"
                    echo "Editing profile: $profile_file"
                    cat $profile_file

                    FLAGS_EDIT_READ=$(sed -n '/^# FLAGS/,$p' "$profile_file")
                    if [[ -z "$FLAGS_EDIT_READ" ]]; then
                        FLAGS_EDIT_READ="# FLAGS"
                    else
                        printf "\n"
                        echo "Remove ALL flags?"
                        printf "[y/N] > "
                        answer="n"
                        IFS= read -r answer || answer=
                        if [[ "${answer,,}" == "y" ]]; then
                            sed -i.bak '/^# FLAGS/,$d' "$profile_file"
                            FLAGS_EDIT_READ="# FLAGS"
                            echo "Removed."
                        fi
                    fi

                    . "$profile_file"
                    break
                fi
                n=$((n + 1))
            done
        fi


        printf 'Select build folder.\n[%s] ' "$BUILD_PATH"
        IFS= read -r answer || answer=
        [ -n "$answer" ] && BUILD_PATH=$answer

        DEFAULT_DEBUG_ANSWER_BOX="[y/N]"
        DEFAULT_DEBUG_ANSWER="N"
        if [[ "$DEBUG" == 1 ]]; then
            DEFAULT_DEBUG_ANSWER_BOX="[Y/n]"
            DEFAULT_DEBUG_ANSWER="Y"
        fi

        printf 'Debug build?\n%s > ' "$DEFAULT_DEBUG_ANSWER_BOX"
        IFS= read -r answer || answer=
        if [[ -z "$answer" ]]; then
            answer="$DEFAULT_DEBUG_ANSWER"
        fi

        if [[ "${answer,,}" == "y" || "${answer,,}" == "debug" ]]; then
            DEBUG=1
            NAME="vivianite-debug"
            NO_CONFIGURE=1
        else
            NAME="vivianite-release"
        fi

        if [[ "$EDIT" == 0 ]]; then
            printf 'select build name.\n[%s] > ' "$name"
            ifs= read -r answer || answer=
            [ -n "$answer" ] && name=$answer
        fi

        printf 'Add any other flags that you want to include separated by space\n(see with --flags option or press Enter to skip).\n> '
        IFS= read -r answer || answer=
        if [[ -n "$answer" || "$EDIT" == 1 ]]; then
            if [[ "$EDIT" == 1 ]]; then
                APPEND_FLAGS=$(printf "%s" "$answer" | sed -e 's/[^ ]*/&=1/g' -e 's/ /\n/g')
                APPEND_FLAGS="$FLAGS_EDIT_READ"\n"$APPEND_FLAGS"
            else
                APPEND_FLAGS=$(printf "# FLAGS\n%s" "$answer" | sed -e 's/[^ ]*/&=1/g' -e 's/ /\n/g')
            fi
        else
            echo "Skipping..."
        fi

        SELECTED=1

        profile_file="$PROFILES_DIR/$NAME.profile"



        if [[ "$EDIT" == 0 ]]; then
            if [ -e "$profile_file" ]; then
                printf 'Profile "%s" already exists. Overwrite? [y/N] ' "$NAME"
                IFS= read -r overwrite || overwrite=
                case "$overwrite" in
                    [Yy]) ;;
                    *)
                        echo "Aborting."
                        exit 1
                        ;;
                esac
            fi
        fi

        APP_PATH="$BUILD_PATH/$NAME"

        {
            printf 'NAME=%s\n' "$(printf '%s' "$NAME" | sed "s/'/'\\\\''/g")"
            printf 'BUILD_PATH=%s\n' "$(printf '%s' "$BUILD_PATH" | sed "s/'/'\\\\''/g")"
            printf 'APP_PATH=%s\n' "$(printf '%s' "$APP_PATH" | sed "s/'/'\\\\''/g")"
            printf '%s\n' "$APPEND_FLAGS"
        } > "$profile_file" || {
            echo "Failed to save profile." >&2
            exit 1
        }
        . "$profile_file"
        if [[ "$EDIT" == 0 ]]; then
            printf "Build %s was succesfully created at %s\n" "$NAME" "$PROFILES_DIR/$NAME.profile" 
        else
            printf "Build \"%s\" was succesfully edited.\n" "$NAME" 
        fi
        ;;

    *)
        case "$RESP" in
            ''|*[!0-9]*)
                echo "Invalid selection."
                exit 1
                ;;
        esac

        if [ "$RESP" -lt 1 ] || [ "$RESP" -gt "$PROFILE_COUNT" ]; then
            echo "Invalid selection."
            exit 1
        fi

        n=1
        for f in "$PROFILES_DIR"/*.profile; do
            if [ "$n" -eq "$RESP" ]; then
                # Source it
                . "$f"
                SELECTED=1
                APP_PATH="$BUILD_PATH/$NAME"
                break
            fi
            n=$((n + 1))
        done
        ;;
esac

if [[ "$SELECTED" != 1 ]]; then
    echo "Build profile not selected."
    exit 1
fi

printf "\n"
if [[ "$EDIT" == 0 ]]; then
    printf 'Selected %s\n' "$NAME"
fi
printf 'Builds folder: %s\n' "$BUILD_PATH"
printf 'Current build folder: %s\n' "$APP_PATH"

if [[ "$DEBUG" == 1 ]]; then
    echo "DEBUG: ON"
fi

if [[ "$FORCE_RECONFIGURE" != 1 && "$NO_RECONFIGURE" == 1 ]]; then
    echo "SKIP CONFIGURE: ON"
fi

if [[ "$JOBS" == 1 ]]; then
    echo "JOBS: ON"
fi

if [[ "$CLEAN" == 1 ]]; then
    echo "CLEAN BUILD: ON"
fi


if [[ "$RUN" == 1 ]]; then
    echo "RUN AFTERWARDS: ON"
fi

if [[ "$INSTALL" == 1 ]]; then
    echo "INSTALL: ON"
else
    if [[ "$CREATE_SHORTCUT" == 1 ]]; then
        echo "CREATE_SHORTCUT: ON"
    fi
fi


printf 'Correct? [Y/n] '

IFS= read -r RESP || RESP=

case "$RESP" in
    ''|[Yy]) ;;
    *)
        echo "Aborting."
        exit 1
        ;;
esac

printf "%s" "$BUILD_PATH" > "$PROFILES_DIR/build_dir.txt"


if [[ "$DELETE" == 1 ]]; then
    printf "Deleting profile \"$NAME\".\n"
    rm "$PROFILES_DIR/$NAME.profile"

    if [[ "${RESP,,}" == "n" ]]; then
        echo "Success."
        exit 1
    fi
    exit 0
fi

if [[ "$EDIT" == 1 ]]; then
    exit 0
fi

LOCAL_BUILD_PATH="$APP_PATH/build"

mkdir -p "$LOCAL_BUILD_PATH" 
(
    CONFIGURE_PATH="$SCRIPT_DIR/configure"
    if [[ "$FORCE_RECONFIGURE" != 1 && "$NO_RECONFIGURE" == 1 && -f "$CONFIGURE_PATH" && -f "$LOCAL_BUILD_PATH/Makefile" ]]; then
        cd "$LOCAL_BUILD_PATH" 
    else
        autoreconf -i
        cd "$LOCAL_BUILD_PATH" 
        $CONFIGURE_PATH
    fi

    if [[ "$DISTCLEAN" == 1 ]]; then
        make distclean
    else
        if [[ "$CLEAN" == 1 ]]; then
            make clean
        fi
    fi

    if [[ "$JOBS" == 1 ]]; then
        make -j
    else
        make
    fi

    echo "Copying assets..."
    cd ..

    GLOBAL_BUILD_PATH=$(pwd)

    if [[ "$INSTALL" == 1 ]]; then
        INSTALL_PATH
        echo "Installing to $INSTALL_DIR/..."
        {
            mkdir -p "$INSTALL_DIR"
            cd "$INSTALL_DIR"
        } || {
            echo "Failed to create install directory, not installing"
        }
    fi

    cp -r $SCRIPT_DIR/modules .
    cp -r $SCRIPT_DIR/assets .
    cp $SCRIPT_DIR/VERSION .
    cp "$GLOBAL_BUILD_PATH/build/vivianite" .
    if [[ "$INSTALL" == 1 ]]; then
        echo "Applicatioin installed."
    fi


    if [[ "$RUN" == 1 ]]; then
        echo "Done. Running vivianite using the following command:"
    else
        echo "Done. Run vivianite using the following command:"
    fi
    echo -e "\t$(pwd)/vivianite"

    SHORTCUT_FILE="$SCRIPT_DIR/Vivianite.desktop"
    if [[ "$CREATE_SHORTCUT" == 1 ]]; then
        {
            printf "#!/usr/bin/env xdg-open\n"
            printf "[Desktop Entry]\n"

            printf "Categories=AudioVideoEditing;AudioVideo;Development;IDE;Utility\n"
            printf "Type=Application\n"
            printf "Name=Vivianite\n"
            printf "Comment=Scripting Video Editor\n"

            printf "Exec=%s\n" "$(pwd)/vivianite"
            printf "Icon=%s\n" "$(pwd)/vivianite/assets/icon/vivianite.png"

        } > $SHORTCUT_FILE || {
            echo "Failed to create a shortcut." >&2
        }
        chmod +x "$SHORTCUT_FILE"
        if [[ "$INSTALL" == 1 ]]; then
            mv "$HORTCUT_FILE" "$XDG_LOCAL_APPS_DIR/"
            echo "Shortcut installed in the system."
        fi

    fi


    if [[ "$RUN" == 1 ]]; then
        ./vivianite
    fi

) || {
    echo "Build failed." >&2
    exit 1
}

