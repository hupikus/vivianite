#!/bin/bash

NAME="vivianite"
PROFILES_FOLDER="build_profiles"

BUILD_PATH="builds"
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"


NAME="vivianite"

SELECTED=0
RESP=""


JOBS=0
if [[ "$*" == *"--jobs"* ]]; then
    JOBS=1
fi

# profile directory
if [ ! -d "$PROFILES_FOLDER" ]; then
    mkdir -p "$PROFILES_FOLDER" || {
        echo "Failed to create profile directory." >&2
        exit 1
    }

    echo "Creating a new build profile."
    RESP="N"
else

    set --
    for f in "$PROFILES_FOLDER"/*.profile; do
        [ -f "$f" ] || continue
        set -- "$@" "$f"
    done

    PROFILE_COUNT=$#

    if [ "$PROFILE_COUNT" -gt 0 ]; then
        echo "Select appropriate build:"
        n=1

        for f do
            profile_name=${f##*/}
            profile_name=${profile_name%.profile}
            printf '[%s] %s\n' "$n" "$profile_name"
            n=$((n + 1))
        done

        echo "[N] Create a new build"
        printf 'Your answer: '
        IFS= read -r RESP || RESP=
    else
        echo "No existing profiles found."
        RESP="N"
    fi
fi

case "$RESP" in
    [Nn])
        printf 'Select build folder. Default: %s\n> ' "$BUILD_PATH"
        IFS= read -r answer || answer=
        [ -n "$answer" ] && BUILD_PATH=$answer

        printf 'Select build name. Default: %s\n> ' "$NAME"
        IFS= read -r answer || answer=
        [ -n "$answer" ] && NAME=$answer

        SELECTED=1

        profile_file="$PROFILES_FOLDER/$NAME.profile"

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

        APP_PATH="$BUILD_PATH/$NAME"

        {
            printf 'NAME=%s\n' "$(printf '%s' "$NAME" | sed "s/'/'\\\\''/g")"
            printf 'BUILD_PATH=%s\n' "$(printf '%s' "$BUILD_PATH" | sed "s/'/'\\\\''/g")"
            printf 'APP_PATH=%s\n' "$(printf '%s' "$APP_PATH" | sed "s/'/'\\\\''/g")"
        } > "$profile_file" || {
            echo "Failed to save profile." >&2
            exit 1
        }
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

        # Select the profile by its one-based menu number.
        n=1
        for f do
            if [ "$n" -eq "$RESP" ]; then
                # Profile files are shell assignments written by this script.
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

echo
printf 'Selected %s\n' "$NAME"
printf 'Builds folder: %s\n' "$BUILD_PATH"
printf 'Current build folder: %s\n' "$APP_PATH"

if [[ "$DEBUG" == 1 ]]; then
    echo "DEBUG: ON"
fi

if [[ "$JOBS" == 1 ]]; then
    echo "JOBS: ON"
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

LOCAL_BUILD_PATH="$APP_PATH/build"

mkdir -p "$LOCAL_BUILD_PATH" || exit 1
(
    autoreconf -i
    cd "$LOCAL_BUILD_PATH" || exit 1
    $SCRIPT_DIR/configure

    if [[ "$JOBS" == 1 ]]; then
        make -j
    else
        make
    fi

    echo "Copying assets..."
    cd ..
    cp -r $SCRIPT_DIR/modules .
    cp -r $SCRIPT_DIR/assets .
    cp $SCRIPT_DIR/VERSION .
    cp build/vivianite .

    echo "Done. Run vivianite using the following command:"
    echo -e "\t$(pwd)/vivianite"
) || {
    echo "Build failed." >&2
    exit 1
}

