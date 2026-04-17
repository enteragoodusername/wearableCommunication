# wearableCommunication
Run in root of project /Awearable:
git submodule update --init --recursive      

# use for generating new secret:
## you must do this if pulling these changes, then replace the "SHARED_SECRET" in secret.hpp before uploading code to receiver and sender since we are not pushing secret to GitHub
python3 -c "import os; print(', '.join(hex(x) for x in os.urandom(32)))"